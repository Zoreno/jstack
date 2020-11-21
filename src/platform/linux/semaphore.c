#include <platform/linux/semaphore.h>

int semaphore_init(semaphore_t *sem, unsigned int value)
{
    return sem_init(&sem->sem, 0, value);
}

int semaphore_wait(semaphore_t *sem)
{
    return sem_wait(&sem->sem);
}

int semaphore_post(semaphore_t *sem)
{
    return sem_post(&sem->sem);
}
