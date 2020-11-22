#ifndef _ARP_H
#define _ARP_H

#include <stdint.h>

#include "device/netdev.h"
#include "ethernet/ethernet.h"

#include "mac_address.h"

#define ARP_ETHERNET 0x0001
#define ARP_IPV4 0x0800
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002

#define ETH_P_IP 0x0800
#define ETH_P_ARP 0x0806
#define ETH_P_IPV6 0x86DD

typedef struct _arp_header
{
    uint16_t hw_type;
    uint16_t protype;
    unsigned char hw_size;
    unsigned char prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed)) arp_header_t;

typedef struct _arp_ipv4
{
    mac_address_t src_mac;
    uint32_t src_ip;
    mac_address_t dst_mac;
    uint32_t dst_ip;
} __attribute__((packed)) arp_ipv4_t;

void arp_init();
void arp_incoming(netdev_t *netdev, eth_header_t *header);
void arp_reply(netdev_t *netdev, eth_header_t *header, arp_header_t *arp_header);
mac_address_t *arp_lookup(uint32_t ip_addr);

#endif