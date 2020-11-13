#include "ethernet/ethernet.h"

#include <string.h>

#include "endianess.h"

eth_header *parse_eth_header(char *buf)
{
    eth_header *header = (eth_header *)buf;

    header->ethertype = htons(header->ethertype);

    return header;
}

eth_header *init_eth_header(eth_header *header, uint16_t ethertype,
                            unsigned char *src_addr, unsigned char *dst_addr)
{
    header->ethertype = htons(ethertype);

    memcpy(header->mac_src, src_addr, 6);
    memcpy(header->mac_dest, dst_addr, 6);

    return header;
}