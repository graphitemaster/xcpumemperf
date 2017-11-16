#include "bsem.h"

int bsem_init(struct bsem* bsem) {
	if (pthread_mutex_init(&bsem->mutex, NULL) < 0) {
		return -1;
	}

	if (pthread_cond_init(&bsem->cond, NULL) < 0) {
		pthread_mutex_destroy(&bsem->mutex);
		return -1;
	}

	bsem->value = 0;
	return 0;
}

void bsem_destroy(struct bsem* bsem) {
	pthread_mutex_destroy(&bsem->mutex);
	pthread_cond_destroy(&bsem->cond);
}

int bsem_post(struct bsem* bsem) {
	if (pthread_mutex_lock(&bsem->mutex) < 0) {
		return -1;
	}
	bsem->value++;
	pthread_cond_signal(&bsem->cond);
	pthread_mutex_unlock(&bsem->mutex);
	return 0;
}

int bsem_wait(struct bsem* bsem) {
	if (pthread_mutex_lock(&bsem->mutex) < 0) {
		return -1;
	}
	while (!bsem->value) {
		pthread_cond_wait(&bsem->cond, &bsem->mutex);
	}
	bsem->value--;
	pthread_mutex_unlock(&bsem->mutex);
	return 0;
}
