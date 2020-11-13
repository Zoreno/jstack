#include "endianess.h"

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

uint32_t htonl(uint32_t hostlong)
{
    return hostlong;
}

uint16_t htons(uint16_t hostshort)
{
    return hostshort;
}

uint32_t ntohl(uint32_t netlong)
{
    return netlong;
}

uint16_t ntohs(uint16_t netshort)
{
    return netshort;
}

#else

static uint32_t swap32(uint32_t val)
{
    return (val & 0xFF) << 24 |
           (val & 0xFF00) << 8 |
           (val & 0xFF0000) >> 8 |
           (val & 0xFF000000) >> 24;
}

static uint16_t swap16(uint16_t val)
{
    return (val & 0xFF) << 8 |
           (val & 0xFF00) >> 8;
}

uint32_t htonl(uint32_t hostlong)
{
    return swap32(hostlong);
}

uint16_t htons(uint16_t hostshort)
{
    return swap16(hostshort);
}

uint32_t ntohl(uint32_t netlong)
{
    return swap32(netlong);
}

uint16_t ntohs(uint16_t netshort)
{
    return swap16(netshort);
}

#endif