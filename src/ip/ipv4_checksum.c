#include "ip/ipv4.h"

#include <stdlib.h>

uint16_t ipv4_calculate_checksum(ipv4_header_t *header)
{
    uint16_t count = header->header_length * 4;
    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)header;

    for (; count > 1; count -= 2)
    {
        sum += *ptr++;
    }

    if (count > 0)
    {
        sum += *(uint8_t *)ptr;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}