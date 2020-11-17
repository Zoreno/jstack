#include "device/netdev.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "device/tap.h"
#include "inet/parse_address.h"

#include "logging.h"

#define _AF_INET 2

void netdev_init(netdev_t *dev, const char *addr, const char *hw_addr)
{
    log_info("Initializing netdev");

    // Clear the memory
    memset(dev, 0, sizeof(netdev_t));

    // Parse the IP address
    if (parse_address(_AF_INET, addr, &dev->addr) != 1)
    {
        log_fatal("Error parsing inet address");
        exit(1);
    }

    // Parse the MAC address
    sscanf(hw_addr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hw_addr[0],
           &dev->hw_addr[1],
           &dev->hw_addr[2],
           &dev->hw_addr[3],
           &dev->hw_addr[4],
           &dev->hw_addr[5]);

    netdev_stats_clear(dev);
}

int netdev_transmit(netdev_t *dev, eth_header_t *hdr, uint16_t ethertype,
                    int len, unsigned char *dst)
{
    init_eth_header(hdr, ethertype, dev->hw_addr, dst);

    len += sizeof(eth_header_t);

    int bytes_sent = dev->send_func((char *)hdr, len);

    dev->stats.tx_bytes += bytes_sent;
    dev->stats.tx_packets += 1;

    return bytes_sent;
}

int netdev_receive(netdev_t *dev, char *dst, int len)
{
    int bytes_read = dev->read_func(dst, len);

    dev->stats.rx_bytes += bytes_read;
    dev->stats.rx_packets += 1;

    return bytes_read;
}

void netdev_stats_clear(netdev_t *netdev)
{
    netdev->stats.tx_bytes = 0;
    netdev->stats.tx_packets = 0;
    netdev->stats.rx_bytes = 0;
    netdev->stats.rx_packets = 0;
}

void netdev_get_stats(netdev_t *netdev, netdev_stats_t *stats)
{
    if (stats != NULL)
    {
        stats = &netdev->stats;
    }
}