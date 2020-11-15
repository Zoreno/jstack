#include "ethernet/ethernet.h"

#include <string.h>
#include <stdio.h>

#include "endianess.h"
#include "logging.h"

static void mac_to_string(char *str, unsigned char *mac)
{
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0],
            mac[1],
            mac[2],
            mac[3],
            mac[4],
            mac[5]);
}

eth_header_t *parse_eth_header_t(char *buf)
{
    eth_header_t *header = (eth_header_t *)buf;

    header->ethertype = htons(header->ethertype);

    char src_mac[18] = {0};
    mac_to_string(src_mac, header->mac_src);
    char dst_mac[18] = {0};
    mac_to_string(dst_mac, header->mac_dest);

    log_debug("Incoming src mac addr: %s", src_mac);
    log_debug("Incoming dst mac addr: %s", dst_mac);

    return header;
}

eth_header_t *init_eth_header_t(eth_header_t *header, uint16_t ethertype,
                                unsigned char *src_addr, unsigned char *dst_addr)
{
    header->ethertype = htons(ethertype);

    unsigned char tmp[6];

    memcpy(tmp, header->mac_dest, 6);
    memcpy(header->mac_dest, header->mac_src, 6);
    memcpy(header->mac_src, tmp, 6);

    //memcpy(header->mac_src, src_addr, 6);
    //memcpy(header->mac_dest, dst_addr, 6);

    char src_mac[18] = {0};
    mac_to_string(src_mac, header->mac_src);
    char dst_mac[18] = {0};
    mac_to_string(dst_mac, header->mac_dest);

    log_debug("Outgoing src mac addr: %s", src_mac);
    log_debug("Outgoing dst mac addr: %s", dst_mac);

    return header;
}