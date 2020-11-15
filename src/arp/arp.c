/**
 * @file arp.c
 * @author Joakim Bertils
 * @brief ARP Implementation
 * @version 0.1
 * @date 2020-11-14
 * 
 * Handle ARP (Address Resolution Protocol) packets.
 */

#include "arp/arp.h"

#include "arp/arp_cache.h"

#include "logging.h"

void arp_init()
{
    log_info("Initalizing ARP module");

    arp_cache_init();
}

unsigned char *arp_lookup(uint32_t ip_addr)
{
    return arp_cache_lookup(ip_addr);
}

//==============================================================================
// End of file
//==============================================================================
