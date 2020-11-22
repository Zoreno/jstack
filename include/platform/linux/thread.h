#ifndef _PLATFORM_LINUX_THREAD_H
#define _PLATFORM_LINUX_THREAD_H

#include <pthread.h>

#define THREAD_NAME_MAX_SIZE 32

typedef void *(*start_func)(void *);

typedef struct _thread
{
    pthread_t thread;
    char name[THREAD_NAME_MAX_SIZE + 1];
} thread_t;

int thread_create(const char *name, thread_t *thread, start_func func, void *arg);
void thread_exit(void *retval);
int thread_join(thread_t *thread, void **retval);
int thread_detach(thread_t *thread);
int thread_equal(thread_t *thread1, thread_t *thread2);
int thread_cancel(thread_t *thread);
thread_t thread_self();
const char const *thread_get_name(thread_t *thread);

#endif