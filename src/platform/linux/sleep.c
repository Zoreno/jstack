#include "platform/linux/sleep.h"

#include <unistd.h>

void thread_sleep(int microseconds)
{
    usleep(microseconds);
}