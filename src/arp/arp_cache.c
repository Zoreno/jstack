#include "arp/arp_cache.h"

#include <string.h>

#include "logging.h"

static arp_cache_entry_t arp_cache[ARP_CACHE_LEN];

void arp_cache_init()
{
    log_info("Initializing ARP cache");

    memset(arp_cache, 0, sizeof(arp_cache));
}

int arp_cache_insert_entry(arp_header_t *header, arp_ipv4_t *data)
{
    arp_cache_entry_t *entry;

    for (int i = 0; i < ARP_CACHE_LEN; ++i)
    {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE)
        {
            log_trace("Inserting value into ARP cache");

            entry->state = ARP_RESOLVED;
            entry->hw_type = header->hw_type;
            entry->src_ip = data->src_ip;
            memcpy(entry->src_mac, data->src_mac, sizeof(entry->src_mac));

            return 0;
        }
    }

    return -1;
}

int arp_cache_update_entry(arp_header_t *header, arp_ipv4_t *data)
{
    arp_cache_entry_t *entry;

    for (int i = 0; i < ARP_CACHE_LEN; ++i)
    {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE)
        {
            continue;
        }

        if (entry->hw_type == header->hw_type &&
            entry->src_ip == data->src_ip)
        {
            log_trace("Updating value in ARP cache");
            memcpy(entry->src_mac, data->src_mac, sizeof(entry->src_mac));
            return -1;
        }
    }

    return 0;
}

int arp_cache_get_capacity()
{
    return ARP_CACHE_LEN;
}

int arp_cache_get_size()
{
    arp_cache_entry_t *entry;
    int len = 0;

    for (int i = 0; i < ARP_CACHE_LEN; ++i)
    {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE)
        {
            continue;
        }

        ++len;
    }

    return len;
}