#include "ip/ipv4.h"

#include "icmp/icmpv4.h"

#include "endianess.h"
#include "logging.h"

void ipv4_print_header(ipv4_header_t *ip_header)
{
    log_debug("=========================================");
    log_debug("header_length: %i", ip_header->header_length);
    log_debug("version: %i", ip_header->version);
    log_debug("type_of_service: 0x%04x", ip_header->type_of_service);
    log_debug("length: %#04x", ip_header->length);

    log_debug("id: %i", ip_header->id);
    log_debug("flags: DF: %i, MF: %i", (ip_header->flags & IPV4_DF) > 0,
              (ip_header->flags & IPV4_MF) > 0);
    log_debug("fragment offset: %i", ip_header->fragment_offset);
    log_debug("Time to live: %i", ip_header->time_to_live);
    log_debug("Protocol: 0x%02x", ip_header->protocol);

    log_debug("Source address: %i.%i.%i.%i",
              ip_header->src_addr_bytes[0],
              ip_header->src_addr_bytes[1],
              ip_header->src_addr_bytes[2],
              ip_header->src_addr_bytes[3]);

    log_debug("Destination address: %i.%i.%i.%i",
              ip_header->dst_addr_bytes[0],
              ip_header->dst_addr_bytes[1],
              ip_header->dst_addr_bytes[2],
              ip_header->dst_addr_bytes[3]);
}
