#include "icmp/icmpv4.h"

#include "ethernet/ethernet.h"
#include "ip/ipv4.h"

#include "endianess.h"
#include "logging.h"

void icmpv4_reply(netdev_t *netdev, eth_header_t *header)
{
    log_info("Sending ICMP echo reply");

    ipv4_header_t *ip_header = (ipv4_header_t *)header->payload;
    icmp_v4_header_t *icmp_header = (icmp_v4_header_t *)ip_header->payload;
    icmp_v4_echo_header_t *echo_header = (icmp_v4_echo_header_t *)icmp_header->data;

    uint16_t icmp_len = ip_header->length - (ip_header->header_length * 4);

    echo_header->id = ntohs(echo_header->id);
    echo_header->seq = ntohs(echo_header->seq);

    icmp_header->type = ICMP_V4_REPLY;
    icmp_header->checksum = 0;
    icmp_header->checksum = icmpv4_calculate_checksum(icmp_header, icmp_len);

    ipv4_outgoing(netdev, header);
}