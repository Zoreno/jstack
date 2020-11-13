#include "arp/arp.h"

#include <string.h>

#include "endianess.h"

void arp_reply(netdev_t *netdev, eth_header *header, arp_header_t *arp_header)
{
    arp_ipv4_t *arp_data;
    int len;

    arp_data = (arp_ipv4_t *)arp_header->data;

    // Source is now destination.
    memcpy(arp_data->dst_mac, arp_data->src_mac, sizeof(arp_data->src_mac));
    memcpy(arp_data->src_mac, netdev->hw_addr, sizeof(netdev->hw_addr));
    arp_data->dst_ip = arp_data->src_ip;
    arp_data->src_ip = netdev->addr;

    arp_header->opcode = ARP_REPLY;

    arp_header->opcode = htons(arp_header->opcode);
    arp_header->hw_type = htons(arp_header->hw_type);
    arp_header->protype = htons(arp_header->protype);

    len = sizeof(arp_header_t) + sizeof(arp_ipv4_t);

    netdev_transmit(netdev, header, ETH_P_ARP, len, arp_data->dst_mac);
}