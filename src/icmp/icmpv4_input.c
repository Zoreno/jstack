#include "icmp/icmpv4.h"

#include "ethernet/ethernet.h"
#include "ip/ipv4.h"

#include "endianess.h"
#include "logging.h"

void icmpv4_incoming(netdev_t *netdev, eth_header_t *header)
{
    ipv4_header_t *ip_header = (ipv4_header_t *)header->payload;
    icmp_v4_header_t *icmp_header = (icmp_v4_header_t *)ip_header->payload;
    icmp_v4_echo_header_t *echo_header = (icmp_v4_echo_header_t *)icmp_header->data;

    echo_header->id = ntohs(echo_header->id);
    echo_header->seq = ntohs(echo_header->seq);

    log_debug("Id: %i", echo_header->id);
    log_debug("Seq: %i", echo_header->seq);

    switch (icmp_header->type)
    {
    case ICMP_V4_ECHO:
        log_info("Got ICMPv4 echo request");
        icmpv4_reply(netdev, header);
        break;
    default:
        log_warn("Got unsupported ICMPv4 message");
        break;
    }
}