#ifndef _PLATFORM_LINUX_SEMAPHORE_H
#define _PLATFORM_LINUX_SEMAPHORE_H

#include <semaphore.h>

typedef struct _semaphore
{
    sem_t sem;

} semaphore_t;

int semaphore_init(semaphore_t *sem, unsigned int value);
int semaphore_wait(semaphore_t *sem);
int semaphore_post(semaphore_t *sem);

#endif