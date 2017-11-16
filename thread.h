#ifndef THREAD_HDR
#define THREAD_HDR

#include "bsem.h"

struct msg;

enum thread_type {
	RD,
	WR
};

struct thread {
	/* Which CPU this thread should run on */
	int cpu;

	/* The type of thread */
	enum thread_type type;

	/* Thread handle */
	pthread_t thread;

	/*
	 * Binary semaphores, one is posted when the thread is started
	 * the other is posted to begin the benchmarking in the thread.
	 */
	struct bsem bsem[2];

	/* Message channel to communicate on */
	struct msg* msg;

	/* The amount of memory to touch */
	size_t memory;
};

/* Initialize a thread */
int thread_init(struct thread *thread, int cpu, enum thread_type type, size_t memory, struct msg *msg);

/* Wait for the thread to get shared memory mapping */
int thread_wait(struct thread *thread, int fd);

/* Start the benchmarking on the thread */
int thread_benchmark(struct thread *thread);

/* Wait for the thread to complete */
int thread_join(struct thread *thread);

/* Destroy thread resources */
void thread_destroy(struct thread *thread);

#endif
