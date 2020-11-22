#include "device/netdev.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "device/tap.h"
#include "inet/parse_address.h"

#include "logging.h"

#define _AF_INET 2

static void *allocate_device_buffer(size_t size)
{
    void *mem = malloc(size);

    if (!mem)
    {
        log_fatal("Failed to allocate memory for device buffer");
        exit(1);
    }

    log_info("Allocated %lu bytes for device", size);

    return mem;
}

void *netdev_rx_thread(void *arg)
{
    netdev_t *tap_device = (netdev_t *)arg;

    // TODO: This should be freed somehow.
    char *buffer = allocate_device_buffer(tap_device->mtu);

    while (1)
    {
        int packet_size = netdev_receive(tap_device, buffer, tap_device->mtu);

        if (packet_size < 0)
        {
            log_warn("ERR: Read from tun_fd: %s", strerror(errno));
        }

        log_debug("Read packet with size %i", packet_size);

        eth_header_t *header = parse_eth_header(buffer);

        ethernet_handle_frame(tap_device, header);
    }

    return NULL;
}

void netdev_init(netdev_t *dev, const char *addr, const char *hw_addr, uint32_t mtu)
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
    mac_address_from_string(&dev->hw_addr, hw_addr);

    netdev_stats_clear(dev);

    dev->addr_len = MAC_ADDRESS_OCTET_COUNT;
    dev->mtu = mtu;
}

int netdev_transmit(netdev_t *dev, eth_header_t *hdr, uint16_t ethertype,
                    int len, mac_address_t *dst)
{
    init_eth_header(hdr, ethertype, &dev->hw_addr, dst);

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

uint32_t netdev_get_mtu(netdev_t *netdev)
{
    return netdev->mtu;
}