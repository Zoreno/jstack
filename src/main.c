#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "arp/arp.h"
#include "device/tap.h"
#include "device/netdev.h"

#include "logging.h"
#include "utils.h"

#define BUFFER_SIZE 128

void handle_frame(netdev_t *netdev, eth_header *header)
{
    switch (header->ethertype)
    {
    case ETH_P_ARP:
        log_info("Got incoming ARP!");
        arp_incoming(netdev, header);
        break;
    case ETH_P_IP:
        log_info("Got IPv4 Packet!");
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

    netdev_t netdev;

    memset(buffer, 0, sizeof(buffer));

    if (tap_init(dev) < 0)
    {
        log_fatal("Failed to create TAP device");
        exit(1);
    }

    log_info("TAP device initialized");

    netdev_init(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");

    arp_init();

    while (1)
    {
        if (tap_read(buffer, BUFFER_SIZE) < 0)
        {
            log_warn("ERR: Read from tun_fd: %s", strerror(errno));
        }

        eth_header *header = parse_eth_header(buffer);

        handle_frame(&netdev, header);
    }
}