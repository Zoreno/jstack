#include "ethernet/ethernet.h"

#include <string.h>
#include <stdio.h>

#include "arp/arp.h"
#include "ip/ipv4.h"

#include "endianess.h"
#include "logging.h"

eth_header_t *parse_eth_header(char *buf)
{
    eth_header_t *header = (eth_header_t *)buf;

    header->ethertype = htons(header->ethertype);

    char src_mac[18] = {0};
    mac_address_to_string(src_mac, &header->mac_src);
    char dst_mac[18] = {0};
    mac_address_to_string(dst_mac, &header->mac_dest);

    log_debug("Incoming src mac addr: %s", src_mac);
    log_debug("Incoming dst mac addr: %s", dst_mac);

    return header;
}

eth_header_t *init_eth_header(eth_header_t *header, uint16_t ethertype,
                              mac_address_t *src_addr, mac_address_t *dst_addr)
{
    header->ethertype = htons(ethertype);

    mac_address_t tmp;

    mac_address_copy(&tmp, &header->mac_dest);
    mac_address_copy(&header->mac_dest, &header->mac_src);
    mac_address_copy(&header->mac_src, &tmp);

    //memcpy(header->mac_src, src_addr, 6);
    //memcpy(header->mac_dest, dst_addr, 6);

    char src_mac[18] = {0};
    mac_address_to_string(src_mac, &header->mac_src);
    char dst_mac[18] = {0};
    mac_address_to_string(dst_mac, &header->mac_dest);

    log_debug("Outgoing src mac addr: %s", src_mac);
    log_debug("Outgoing dst mac addr: %s", dst_mac);

    return header;
}

void ethernet_handle_frame(netdev_t *netdev, eth_header_t *header)
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