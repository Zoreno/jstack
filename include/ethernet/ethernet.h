#ifndef _ETHERNET_H
#define _ETHERNET_H

#include <stdint.h>

typedef struct _eth_header_t
{
    unsigned char mac_dest[6];
    unsigned char mac_src[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed)) eth_header_t;

eth_header_t *parse_eth_header(char *buf);
eth_header_t *init_eth_header(eth_header_t *header, uint16_t ethertype,
                              unsigned char *src_addr, unsigned char *dst_addr);

#endif