/**
 * @file ipv4.h
 * @author Joakim Bertils
 * @brief IPv4 Layer interface.
 * @version 0.1
 * @date 2020-11-14
 * 
 * 
 */

#ifndef _IPV4_H
#define _IPV4_H

#include <stdint.h>

#include "device/netdev.h"
#include "ethernet/ethernet.h"

//==============================================================================
// Definitions
//==============================================================================

#define IPV4 0x04
#define ICMPV4 0x01

#define IPV4_MF 0x1
#define IPV4_DF 0x2

typedef struct _ipv4_header
{
    uint8_t header_length : 4;
    uint8_t version : 4;
    uint8_t type_of_service;
    uint16_t length;

    uint16_t id;
    union
    {
        struct
        {
            uint16_t fragment_offset : 13;
            uint16_t flags : 3;
        };
        uint16_t bitfield2;
    };

    uint8_t time_to_live;
    uint8_t protocol;
    uint16_t checksum;

    union
    {
        uint32_t src_addr;
        uint8_t src_addr_bytes[4];
    };

    union
    {
        uint32_t dst_addr;
        uint8_t dst_addr_bytes[4];
    };

    uint8_t payload[];
} __attribute__((packed)) ipv4_header_t;

//==============================================================================
// Interface functions
//==============================================================================

void ipv4_incoming(netdev_t *netdev, eth_header *header);
void ipv4_outgoing(netdev_t *netdev, eth_header *header);

/**
 * @brief Calculates the IPv4 checksum over the packet.
 *
 * From wikipedia:
 *
 * The checksum field is the 16-bit one's complement of the one's complement sum
 * of all 16 bit words in the header. The checksum field should be zero when
 * calculating the checksum and the result of calculating the checksum of a
 * valid packet should be zero.
 *
 * @param header Pointer to IPv4 header to calculate/verify checksum of.
 * @return Checksum of the packet.
 */
uint16_t ipv4_calculate_checksum(ipv4_header_t *header);

#endif

//==============================================================================
// End of file
//==============================================================================
