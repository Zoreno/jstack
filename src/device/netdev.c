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
}

void netdev_transmit(netdev_t *dev, eth_header *hdr, uint16_t ethertype,
                     int len, unsigned char *dst)
{
    init_eth_header(hdr, ethertype, dev->hw_addr, dst);

    len += sizeof(eth_header);

    tap_write((char *)hdr, len);
}