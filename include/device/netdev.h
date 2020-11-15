#ifndef _NETDEV_H
#define _NETDEV_H

#include <stdint.h>

#include "ethernet/ethernet.h"

typedef int (*netdev_send_t)(char *, int);
typedef int (*netdev_read_t)(char *, int);

typedef struct _netdev
{
    uint32_t addr;
    unsigned char hw_addr[6];

    netdev_send_t send_func;
    netdev_read_t read_func;
} netdev_t;

void netdev_init(netdev_t *dev, const char *addr, const char *hw_addr);
int netdev_transmit(netdev_t *dev, eth_header_t *hdr, uint16_t ethertype, int len, unsigned char *dst);
int netdev_receive(netdev_t *dev, char *buffer, int len);

#endif