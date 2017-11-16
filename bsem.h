#ifndef BSEM_HDR
#define BSEM_HDR

#include <pthread.h>

/* Simple binary semaphore with pthread mutex and condition variable */
struct bsem {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int value;
};

int bsem_init(struct bsem* bsem);
void bsem_destroy(struct bsem* bsem);
int bsem_post(struct bsem* bsem);
int bsem_wait(struct bsem* bsem);

#endif
