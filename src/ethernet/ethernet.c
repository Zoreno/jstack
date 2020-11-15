#include "ethernet/ethernet.h"

#include <string.h>

#include "endianess.h"

eth_header_t *parse_eth_header_t(char *buf)
{
    eth_header_t *header = (eth_header_t *)buf;

    header->ethertype = htons(header->ethertype);

    return header;
}

eth_header_t *init_eth_header_t(eth_header_t *header, uint16_t ethertype,
                            unsigned char *src_addr, unsigned char *dst_addr)
{
    header->ethertype = htons(ethertype);

    memcpy(header->mac_src, src_addr, 6);
    memcpy(header->mac_dest, dst_addr, 6);

    return header;
}