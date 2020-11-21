#include <platform/linux/thread.h>

int thread_create(thread_t *thread, start_func func, void *arg)
{
    return pthread_create(&thread->thread, NULL, func, arg);
}

void thread_exit(void *retval)
{
    pthread_exit(retval);
}

int thread_join(thread_t *thread, void **retval)
{
    return pthread_join(thread->thread, retval);
}

int thread_detach(thread_t *thread)
{
    return pthread_detach(thread->thread);
}

int thread_equal(thread_t *thread1, thread_t *thread2)
{
    return pthread_equal(thread1->thread, thread2->thread);
}

int thread_cancel(thread_t *thread)
{
    return pthread_cancel(thread->thread);
}

thread_t thread_self()
{
    thread_t ret;

    ret.thread = pthread_self();

    return ret;
}
