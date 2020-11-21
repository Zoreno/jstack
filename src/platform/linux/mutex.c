#include <platform/linux/mutex.h>

#include <pthread.h>

// TODO: We may need to create an abstraction for the error codes in this
// module, as the different systems may differ.

int mutex_init(mutex_t *mutex)
{
    // TODO: Handle mutex attrs
    return pthread_mutex_init(&mutex->mutex, NULL);
}

int mutex_destroy(mutex_t *mutex)
{
    return pthread_mutex_destroy(&mutex->mutex);
}

int mutex_lock(mutex_t *mutex)
{
    return pthread_mutex_lock(&mutex->mutex);
}

int mutex_trylock(mutex_t *mutex)
{
    return pthread_mutex_trylock(&mutex->mutex);
}

int mutex_unlock(mutex_t *mutex)
{
    return pthread_mutex_unlock(&mutex->mutex);
}