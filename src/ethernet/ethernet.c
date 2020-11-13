#include "ethernet/ethernet.h"

#include "endianess.h"

eth_header *init_eth_header(char *buf)
{
    eth_header *header = (eth_header *)buf;

    header->ethertype = htons(header->ethertype);

    return header;
}