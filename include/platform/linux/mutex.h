#ifndef _PLATFORM_LINUX_MUTEX_H
#define _PLATFORM_LINUX_MUTEX_H

#include <pthread.h>

typedef struct _mutex
{
    pthread_mutex_t mutex;
} mutex_t;

int mutex_init(mutex_t *mutex);
int mutex_destroy(mutex_t *mutex);
int mutex_lock(mutex_t *mutex);
int mutex_trylock(mutex_t *mutex);
int mutex_unlock(mutex_t *mutex);

#endif