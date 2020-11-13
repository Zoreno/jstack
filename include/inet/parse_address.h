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
 * @brief Parses a IP address from presentation to binary representation.
 * 
 * For example IPv4:
 * '127.0.0.1' is converted to '0x7F 0x00 0x00 0x01'.
 * 
 * IPv6:
 * fe00::1 is converted to 0xFE00 0x0000 0x0000 0x0000 0x0000 0x0001.
 * 
 * @param af AF_INET for IPv4 or AF_INET6 for IPv6 address.
 * @param src Ip address string.
 * @param dst Pointer to where to store the result.
 * @return 1 if valid, otherwise 0.
 */
int parse_address(int af, const char *src, void *dst);

#endif

//==============================================================================
// End of file
//==============================================================================
