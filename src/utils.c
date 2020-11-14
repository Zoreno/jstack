/**
 * @file utils.c
 * @author Joakim Bertils
 * @brief Utility routines.
 * @version 0.1
 * @date 2020-11-12
 * 
 * 
 */

#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "logging.h"

int command(char *command_format, ...)
{
    va_list arg_list;
    char command_buffer[COMMAND_BUFFER_LENGTH];
    va_start(arg_list, command_format);
    vsnprintf(command_buffer, COMMAND_BUFFER_LENGTH, command_format, arg_list);
    va_end(arg_list);

    log_info("Running system command: %s", command_buffer);

    return system(command_buffer);
}

//==============================================================================
// End of file.
//==============================================================================
