#ifndef _ICMPV4_H
#define _ICMPV4_H

#include <stdint.h>

#include "device/netdev.h"
#include "ethernet/ethernet.h"

#define ICMP_V4_REPLY 0x00
#define ICMP_V4_DST_UNREACHABLE 0x03
#define ICMP_V4_SRC_QUENCH 0x04
#define ICMP_V4_REDIRECT 0x05
#define ICMP_V4_ECHO 0x08
#define ICMP_V4_ROUTER_ADV 0x09
#define ICMP_V4_ROUTER_SOL 0x0a
#define ICMP_V4_TIMEOUT 0x0b
#define ICMP_V4_MALFORMED 0x0c

typedef struct _icmp_v4_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) icmp_v4_header_t;

typedef struct _icmp_v4_echo_header
{
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed)) icmp_v4_echo_header_t;

typedef struct _icmp_v4_dst_unreachable_header
{
    uint8_t unused;
    uint8_t length;
    uint16_t var;
    uint8_t data[];
} __attribute__((packed)) icmp_v4_dst_unreachable_header_t;

void icmpv4_incoming(netdev_t *netdev, eth_header_t *header);
void icmpv4_reply(netdev_t *netdev, eth_header_t *header);

uint16_t icmpv4_calculate_checksum(icmp_v4_header_t *header, uint16_t size);

#endif