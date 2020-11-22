#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <stdint.h>

#include "mac_address.h"

typedef struct _netdev netdev_t;

typedef struct _eth_header_t
{
    mac_address_t mac_dest;
    mac_address_t mac_src;
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed)) eth_header_t;

eth_header_t *parse_eth_header(char *buf);
eth_header_t *init_eth_header(eth_header_t *header, uint16_t ethertype,
                              mac_address_t *src_addr, mac_address_t *dst_addr);
void ethernet_handle_frame(netdev_t *netdev, eth_header_t *header);

#endif