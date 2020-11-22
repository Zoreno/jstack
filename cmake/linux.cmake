
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(jstack PRIVATE Threads::Threads)

target_sources(jstack PRIVATE src/platform/linux/mutex.c)
target_sources(jstack PRIVATE src/platform/linux/semaphore.c)
target_sources(jstack PRIVATE src/platform/linux/sleep.c)
target_sources(jstack PRIVATE src/platform/linux/thread.c)

