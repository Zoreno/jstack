#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "arp/arp.h"
#include "device/tap.h"
#include "device/netdev.h"
#include "ip/ipv4.h"

#include "logging.h"
#include "utils.h"

#define BUFFER_SIZE 128

void handle_frame(netdev_t *netdev, eth_header_t *header)
{
    switch (header->ethertype)
    {
    case ETH_P_ARP:
        log_info("Got incoming ARP!");
        arp_incoming(netdev, header);
        break;
    case ETH_P_IP:
        ipv4_incoming(netdev, header);
        break;
    case ETH_P_IPV6:
        log_trace("Got IPv6 Packet!");
        break;
    default:
        log_warn("ETH: Unrecognized ethernet type: %#04x",
                 header->ethertype);
        break;
    }
}

int main()
{
    char buffer[BUFFER_SIZE];
    char *dev = calloc(10, 1);

    memset(buffer, 0, sizeof(buffer));

    if (tap_init(dev) < 0)
    {
        log_fatal("Failed to create TAP device");
        exit(1);
    }

    log_info("TAP device initialized");

    netdev_t tap_device;
    netdev_init(&tap_device, "10.0.0.4", "00:0c:29:6d:50:25");
    tap_device.read_func = tap_read;
    tap_device.send_func = tap_write;

    arp_init();

    while (1)
    {
        if (netdev_receive(&tap_device, buffer, BUFFER_SIZE) < 0)
        {
            log_warn("ERR: Read from tun_fd: %s", strerror(errno));
        }

        eth_header_t *header = parse_eth_header_t(buffer);

        handle_frame(&tap_device, header);
    }
}