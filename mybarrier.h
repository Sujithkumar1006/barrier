#ifndef _MYBARRIER_H
#define _MYBARRIER_H

#include <pthread.h>
#include <errno.h>

typedef struct _mybarrier_t
{
    int count;
    int waiting;
    int used;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} mybarrier_t;

mybarrier_t *mybarrier_init(unsigned int count);
int mybarrier_destroy(mybarrier_t *barrier);
int mybarrier_wait(mybarrier_t *barrier);

#endif // #ifndef _MYBARRIER_H
