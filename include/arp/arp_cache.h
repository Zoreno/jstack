#ifndef _ARP_CACHE_H
#define _ARP_CACHE_H

#include "arp/arp.h"

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

typedef struct _arp_cache_entry
{
    uint16_t hw_type;
    uint32_t src_ip;
    unsigned char src_mac[6];
    unsigned int state;
} arp_cache_entry_t;

void arp_cache_init();
int arp_cache_insert_entry(arp_header_t *header, arp_ipv4_t *data);
int arp_cache_update_entry(arp_header_t *header, arp_ipv4_t *data);
int arp_cache_get_capacity();
int arp_cache_get_size();
unsigned char *arp_cache_lookup(uint32_t ip_addr);

#endif