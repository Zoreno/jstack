/**
 * @file ipv4_input.c
 * @author Joakim Bertils
 * @brief Handling of all incoming IPv4 packets.
 * @version 0.1
 * @date 2020-11-14
 * 
 * 
 */

#include "ip/ipv4.h"

#include "icmp/icmpv4.h"

#include "endianess.h"
#include "logging.h"

//==============================================================================
// Local functions
//==============================================================================

//==============================================================================
// Interface functions
//==============================================================================

void ipv4_incoming(netdev_t *netdev, eth_header_t *header)
{
    ipv4_header_t *ip_header = (ipv4_header_t *)header->payload;
    uint16_t checksum = -1;

    // Note that the header length is measured in 32 bit words.
    checksum = ipv4_calculate_checksum(ip_header);

    if (checksum != 0)
    {
        log_error("IPv4 packet corruption detected: Invalid Checksum");
        return;
    }

    ip_header->length = ntohs(ip_header->length);
    ip_header->id = ntohs(ip_header->id);
    ip_header->bitfield2 = ntohs(ip_header->bitfield2);

    //ipv4_print_header(ip_header);

    if (ip_header->version != IPV4)
    {
        log_error("Packet was not IPv4");
        return;
    }

    if (ip_header->header_length < 5)
    {
        log_error("IPv4 header too small");
        return;
    }

    if (ip_header->time_to_live == 0)
    {
        log_error("IPv4 packet TTL reached 0");
        return;
    }

    if ((ip_header->flags & IPV4_MF) > 0)
    {
        log_warn("IPv4 Fragmentation detected but not supported");
        return;
    }

    switch (ip_header->protocol)
    {
    case ICMPV4:
        log_info("Incoming ICMPV4 Packet!");
        icmpv4_incoming(netdev, header);
        break;
    default:
        log_warn("Unknown IP header protocol: %#02x", ip_header->protocol);
        return;
    }
}

//==============================================================================
// End of file.
//==============================================================================
