/**
 * @file utils.h
 * @author Joakim Bertils
 * @brief Utility routines.
 * @version 0.1
 * @date 2020-11-12
 * 
 * Utility routines for performing misc tasks.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

/**
 * @brief Max buffer size for the command expansion.
 * 
 */
#define COMMAND_BUFFER_LENGTH 1024

/**
 * @brief Run a custom system command
 * 
 * @param command_format printf-formatted command string.
 * @param ... Additional parameters to fill out the format.
 * @return System command return value.
 */
int command(char *command_format, ...);
void hexdump(char *str, int len);

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

#endif

//==============================================================================
// End of file.
//==============================================================================
