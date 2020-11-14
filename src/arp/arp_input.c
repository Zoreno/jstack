#include "arp/arp.h"

#include <stdio.h>

#include "arp/arp_cache.h"

#include "endianess.h"
#include "logging.h"

void arp_incoming(netdev_t *netdev, eth_header *header)
{
    arp_header_t *arp_header = (arp_header_t *)header->payload;

    arp_header->hw_type = ntohs(arp_header->hw_type);
    arp_header->protype = ntohs(arp_header->protype);
    arp_header->opcode = ntohs(arp_header->opcode);

    if (arp_header->hw_type != ARP_ETHERNET)
    {
        log_warn("ARP: Unsupported HW type");
        return;
    }

    if (arp_header->protype != ARP_IPV4)
    {
        log_warn("ARP: Unsupported protocol");
        return;
    }

    arp_ipv4_t *arp_data = (arp_ipv4_t *)arp_header->data;

    int merge = arp_cache_update_entry(arp_header, arp_data);

    if (netdev->addr != arp_data->dst_ip)
    {
        log_info("ARP: Packet not for us");
    }

    if (!merge && arp_cache_insert_entry(arp_header, arp_data) != 0)
    {
        // TODO: Implent LRU cache replacement policy
        log_error("ARP: No space in ARP cache!");
    }

    switch (arp_header->opcode)
    {
    case ARP_REQUEST:
        arp_reply(netdev, header, arp_header);
        break;
    default:
        log_warn("ARP: Opcode not supported");
        break;
    }
}