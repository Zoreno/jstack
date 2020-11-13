#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "arp/arp.h"
#include "device/tap.h"
#include "device/netdev.h"

#include "utils.h"

#define BUFFER_SIZE 128

void handle_frame(netdev_t *netdev, eth_header *header)
{
    switch (header->ethertype)
    {
    case ETH_P_ARP:
        printf("Got incoming ARP!\n");
        arp_incoming(netdev, header);
        break;
    case ETH_P_IP:
        printf("Got IPv4 Packet!\n");
        break;
    case ETH_P_IPV6:
        // printf("Got IPv6 Packet!\n");
        break;
    default:
        fprintf(stderr, "ETH: Unrecognized ethernet type: %#04x\n",
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
        fprintf(stderr, "Failed to create TAP device\n");
        exit(1);
    }

    netdev_init(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");

    arp_init();

    while (1)
    {
        if (tap_read(buffer, BUFFER_SIZE) < 0)
        {
            fprintf(stderr, "ERR: Read from tun_fd: %s\n", strerror(errno));
        }

        eth_header *header = parse_eth_header(buffer);

        handle_frame(&netdev, header);
    }
}