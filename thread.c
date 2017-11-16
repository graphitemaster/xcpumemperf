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
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(n, &set);
	if (pthread_setaffinity_np(pthread_self(), sizeof set, &set) < 0) {
		fprintf(stderr, "failed to set thread '%s' to cpu '%d'\n", which, n);
		return -1;
	}
	struct sched_param p = {
		.sched_priority = 99 /* Max priority */
	};
	if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &p) < 0) {
		fprintf(stderr, "failed to set thread '%s' priority", which);
		return -1;
	}
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
	bsem_post(&thread->bsem);

	if (is_rd) {
		for (size_t i = 0; i < thread->memory; i++) {
			load(touch + i);
		}
	} else {
		for (size_t i = 0; i < thread->memory; i++) {
			store(touch + i, i);
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
	bsem_post(&thread->bsem);

	return NULL;
}

int thread_init(struct thread *thread, int cpu, enum thread_type type, size_t memory, struct msg *msg) {
	thread->type = type;
	thread->msg = msg;
	thread->memory = memory;
	thread->cpu = cpu;
	/* Initialize binary semaphore used to know when the thread has started */
	if (bsem_init(&thread->bsem) < 0) {
		return -1;
	}
	/* Create the thread and execute it */
	if (pthread_create(&thread->thread, NULL, proc, thread) < 0) {
		bsem_destroy(&thread->bsem);
		return -1;
	}
	return 0;
}

int thread_wait(struct thread *thread, int fd) {
	/* Write the shared memory file descriptor to the message channel for the thread */
	if (msg_sendfd(thread->msg, fd) < 0) {
		return -1;
	}
	/* Wait for the thread to get shared memory and start */
	if (bsem_wait(&thread->bsem) < 0) {
		return -1;
	}
	return 0;
}

void thread_join(struct thread *thread) {
	pthread_join(thread->thread, NULL);
}

void thread_destroy(struct thread *thread) {
	bsem_destroy(&thread->bsem);
}
