#include "arp/arp.h"

#include <stdio.h>

#include "arp/arp_cache.h"

#include "endianess.h"

void arp_incoming(netdev_t *netdev, eth_header *header)
{
    arp_header_t *arp_header = (arp_header_t *)header->payload;

    arp_header->hw_type = ntohs(arp_header->hw_type);
    arp_header->protype = ntohs(arp_header->protype);
    arp_header->opcode = ntohs(arp_header->opcode);

    if (arp_header->hw_type != ARP_ETHERNET)
    {
        fprintf(stderr, "ARP: Unsupported HW type\n");
        return;
    }

    if (arp_header->protype != ARP_IPV4)
    {
        fprintf(stderr, "ARP: Unsupported protocol\n");
    }

    arp_ipv4_t *arp_data = (arp_ipv4_t *)arp_header->data;

    int merge = arp_cache_update_entry(arp_header, arp_data);

    if (netdev->addr != arp_data->dst_ip)
    {
        printf("ARP: Packet not for us.\n");
    }

    if (!merge && arp_cache_insert_entry(arp_header, arp_data) != 0)
    {
        // TODO: Implent LRU cache replacement policy
        fprintf(stderr, "ARP: No space in ARP cache!\n");
    }

    switch (arp_header->opcode)
    {
    case ARP_REQUEST:
        arp_reply(netdev, header, arp_header);
        break;
    default:
        fprintf(stderr, "ARP: Opcode not supported\n");
        break;
    }
}