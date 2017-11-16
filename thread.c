#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "thread.h"
#include "msg.h"

/*
 * Set which CPU a thread will execute on and force the priority of it
 * to the largest priority to reduce scheduling noise.
 */
static int setcpuandprio(const char* which, int n) {
	/* Pin the thread under the specified CPU */
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(n, &set);
	if (pthread_setaffinity_np(pthread_self(), sizeof set, &set) < 0) {
		fprintf(stderr, "failed to set thread '%s' to cpu '%d'\n", which, n);
		return -1;
	}
	/* Force thread to reschedule */
	sched_yield();
	return 0;
}

/*
 * Functions to prevent the compiler from removing or reordering loads
 * and stores or optimizing them away, these behave as full barriers.
 */
static unsigned char __attribute__ ((noinline)) load(unsigned char *x) {
	return *x;
}

static void __attribute__ ((noinline)) store(unsigned char *x, unsigned char y) {
	*x = y;
}

static void *proc(void *opaque) {
	struct thread *thread = opaque;

	int fd = -1;
	int is_rd = thread->type == RD;
	const char *which = is_rd ? "rd" : "wr";

	/* Set the CPU this thread will run on */
	if (setcpuandprio(which, thread->cpu) < 0) {
		goto error;
	}

	/* Get the shared memory file descriptor from the message channel */
	fd = msg_readfd(thread->msg);
	if (fcntl(fd, F_GETFD) < 0) {
		fprintf(stderr, "%s got invalid file descriptor for shared memory\n", which);
		goto error;
	}

	/* Map in the shared memory */
	int prot = is_rd ? PROT_READ : PROT_WRITE;
	unsigned char *touch = mmap(NULL, thread->memory, prot, MAP_SHARED, fd, 0);
	if (touch == MAP_FAILED) {
		fprintf(stderr, "failed to map shared memory for %s thread\n", which);
		goto error;
	}

	/* Inform the main process that this thread is ready */
	bsem_post(&thread->bsem[0]);

	/* Wait to begin timing */
	bsem_wait(&thread->bsem[1]);
	if (is_rd) {
		volatile unsigned char __attribute__ ((unused)) sink = 0;
		for (size_t i = 0; i < thread->memory; i++) {
			sink = load(touch + i);
			__sync_synchronize();
		}
	} else {
		for (size_t i = 0; i < thread->memory; i++) {
			store(touch + i, i & 0xFF);
			__sync_synchronize();
		}
	}

	/* No longer need the shared memory mapping */
	munmap(touch, thread->memory);
	close(fd);

	return NULL;

error:
	if (fd) {
		close(fd);
	}

	/* Don't dead lock during initialization if an error was encountered */
	bsem_post(&thread->bsem[0]);

	return NULL;
}

int thread_init(struct thread *thread, int cpu, enum thread_type type, size_t memory, struct msg *msg) {
	thread->type = type;
	thread->msg = msg;
	thread->memory = memory;
	thread->cpu = cpu;
	/* Initialize binary semaphores used for controlling when things happen*/
	if (bsem_init(&thread->bsem[0]) < 0) {
		goto error_bsem0;
	}
	if (bsem_init(&thread->bsem[1]) < 0) {
		goto error_bsem1;
	}
	/* Create the thread and execute it */
	if (pthread_create(&thread->thread, NULL, proc, thread) < 0) {
		goto error_bsem1;
	}
	return 0;

error_bsem1:
	bsem_destroy(&thread->bsem[1]);
error_bsem0:
	bsem_destroy(&thread->bsem[0]);
	return -1;
}

int thread_wait(struct thread *thread, int fd) {
	/* Write the shared memory file descriptor to the message channel for the thread */
	if (msg_sendfd(thread->msg, fd) < 0) {
		return -1;
	}
	/* Wait for the thread to get shared memory and start */
	if (bsem_wait(&thread->bsem[0]) < 0) {
		return -1;
	}
	return 0;
}

int thread_benchmark(struct thread* thread) {
	/* Wake up the thread to begin benchmarking */
	return bsem_post(&thread->bsem[1]);
}

int thread_join(struct thread *thread) {
	return pthread_tryjoin_np(thread->thread, NULL);
}

void thread_destroy(struct thread *thread) {
	bsem_destroy(&thread->bsem[0]);
	bsem_destroy(&thread->bsem[1]);
}
