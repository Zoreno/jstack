#ifndef _NETDEV_H
#define _NETDEV_H

#include <stdint.h>

#include "ethernet/ethernet.h"

#include "mac_address.h"

typedef int (*netdev_send_t)(char *, int);
typedef int (*netdev_read_t)(char *, int);

typedef struct _netdev_stats
{
    uint64_t tx_bytes;
    uint64_t rx_bytes;
    uint64_t tx_packets;
    uint64_t rx_packets;
} netdev_stats_t;

typedef struct _netdev
{
    uint32_t addr;
    mac_address_t hw_addr;

    netdev_send_t send_func;
    netdev_read_t read_func;

    netdev_stats_t stats;

    uint8_t addr_len;
    uint32_t mtu;
} netdev_t;

void *netdev_rx_thread(void *arg);
void netdev_init(netdev_t *dev, const char *addr, const char *hw_addr, uint32_t mtu);
int netdev_transmit(netdev_t *dev, eth_header_t *hdr, uint16_t ethertype, int len, mac_address_t *dst);
int netdev_receive(netdev_t *dev, char *buffer, int len);
void netdev_stats_clear(netdev_t *netdev);
void netdev_get_stats(netdev_t *netdev, netdev_stats_t *stats);
uint32_t netdev_get_mtu(netdev_t *netdev);

#endif