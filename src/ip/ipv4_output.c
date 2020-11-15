#include "ip/ipv4.h"

#include "endianess.h"

#define ETH_P_IP 0x0800

void ipv4_outgoing(netdev_t *netdev, eth_header_t *header)
{
    ipv4_header_t *ip_header = (ipv4_header_t *)header->payload;
    uint16_t length = ip_header->length;

    // TODO: This should be looked up in the ARP cache
    uint32_t tmp_addr = ip_header->src_addr;
    ip_header->src_addr = netdev->addr;
    ip_header->dst_addr = tmp_addr;

    ipv4_print_header(ip_header);

    ip_header->length = htons(ip_header->length);
    ip_header->id = htons(ip_header->id);
    ip_header->bitfield2 = htons(ip_header->bitfield2);

    // The checksum should be zeroed before calculation.
    ip_header->checksum = 0;
    ip_header->checksum = ipv4_calculate_checksum(ip_header);

    netdev_transmit(netdev, header, ETH_P_IP, length, header->mac_src);
}