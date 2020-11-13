#ifndef _NETDEV_H
#define _NETDEV_H

#include <stdint.h>

#include "ethernet/ethernet.h"

typedef struct _netdev
{
    uint32_t addr;
    unsigned char hw_addr[6];
} netdev_t;

void netdev_init(netdev_t *dev, const char *addr, const char *hw_addr);
void netdev_transmit(netdev_t *dev, eth_header *hdr, uint16_t ethertype, int len, unsigned char *dst);

#endif