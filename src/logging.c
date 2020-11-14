/**
 * @file logging.c
 * @author Joakim Bertils
 * @brief Logging implementation
 * @version 0.1
 * @date 2020-11-14
 * 
 * Implementation of the logging module.
 */

#include "logging.h"

//==============================================================================
// Local variables
//==============================================================================

static const char *level_strings[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
    "\x1b[94m",
    "\x1b[36m",
    "\x1b[32m",
    "\x1b[33m",
    "\x1b[31m",
    "\x1b[35m"};
#endif

static log_level_t level_threshold = LOG_INFO;

//==============================================================================
// Local functions
//==============================================================================

/**
 * @brief Get the current time formatted as a HH:MM:SS
 * 
 * @param buffer Pointer to where to store the time.
 * @param buffer_size Size of the buffer to store the time.
 * 
 * @return -1 on error, otherwise 0
 */
static int get_current_time(char *buffer, size_t buffer_size)
{
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);

    size_t endp = strftime(buffer, buffer_size, "%H:%M:%S", ts);

    if (endp == 0)
    {
        return -1;
    }

    buffer[endp] = '\0';

    return 0;
}

//==============================================================================
// Interface functions
//==============================================================================

void log_printf(log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level < level_threshold)
    {
        return;
    }

    char buf[16];

    if (get_current_time(buf, sizeof(buf)) == -1)
    {
        fprintf(stderr, "Buffer too small to format date");
    }

#ifdef LOG_USE_COLOR
    printf("%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
           buf, level_colors[level], level_strings[level], file, line);
#else
    printf("%s %-5s %s:%d: ",
           buf, level_strings[level], file, line);
#endif
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    printf("\n");
    fflush(stdout);
}

const char *log_level_string(log_level_t level)
{
    return level_strings[level];
}

void log_set_threshold(log_level_t level)
{
    level_threshold = level;
}

log_level_t log_get_threshold()
{
    return level_threshold;
}

//==============================================================================
// End of file.
//==============================================================================
