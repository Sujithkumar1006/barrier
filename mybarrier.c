#include <stdlib.h>
#include "mybarrier.h"
#include <stdio.h>

mybarrier_t *mybarrier_init(unsigned int count)
{
	mybarrier_t *barrier = malloc(sizeof(*barrier));

	if (NULL == barrier)
	{
		return NULL;
	}

	barrier->count = count;
	barrier->waiting = 0;

	if (pthread_mutex_init(&barrier->mutex, NULL) != 0)
	{
		free(barrier);
		return NULL;
	}
	if (pthread_cond_init(&barrier->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&barrier->mutex);
		free(barrier);
		return NULL;
	}

	return barrier;
}

int mybarrier_destroy(mybarrier_t *barrier)
{

	pthread_mutex_lock(&barrier->mutex);
	// printf("Barried, %d and %d  and %dis \n", barrier->count,barrier->waiting, barrier->used);
	while (barrier->waiting > 0)
	{
		pthread_cond_broadcast(&barrier->cond);
	}
	pthread_mutex_unlock(&barrier->mutex);
	pthread_mutex_destroy(&barrier->mutex);
	pthread_cond_destroy(&barrier->cond);
	free(barrier);

	return 0;
}

int mybarrier_wait(mybarrier_t *barrier)
{
	if (barrier == NULL)
	{
		return -1;
	}
	pthread_mutex_lock(&barrier->mutex);
	// printf("Barried, %d and %d  and %dis \n", barrier->count,barrier->waiting, barrier->used);
	barrier->waiting++;

	if (barrier->waiting < barrier->count && barrier->used == 0)
	{
		pthread_cond_wait(&barrier->cond, &barrier->mutex);
	}
	else if (barrier->waiting == barrier->count)
	{
		barrier->used = 1;
		barrier->waiting = 0;
		pthread_cond_broadcast(&barrier->cond);
	}
	else
	{
		pthread_mutex_unlock(&barrier->mutex);
		barrier->waiting--;
		return -1;
	}
	pthread_mutex_unlock(&barrier->mutex);
	return 0;
}
