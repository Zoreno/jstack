/**
 * @file parse_address.h
 * @author Joakim Bertils
 * @brief Functions for parsing IP addresses
 * @version 0.1
 * @date 2020-11-12
 * 
 * 
 */

#ifndef _PARSE_ADDRESS_H
#define _PARSE_ADDRESS_H

//==============================================================================
// Interface functions
//==============================================================================

/**
 * @brief Parses a IP address from presentation to binary
 * 
 * @param af AF_INET or AF_INET6
 * @param src Ip address string
 * @param dst Pointer to where to store the result.
 * @return 1 if valid, otherwise 0.
 */
int parse_address(int af, const char *src, void *dst);

#endif

//==============================================================================
// End of file
//==============================================================================
