#include "arp/arp_cache.h"

#include <string.h>

#include "logging.h"

static arp_cache_entry_t arp_cache[ARP_CACHE_LEN];

static const char *hw_type_to_string(uint16_t hw_type)
{
    switch (hw_type)
    {
    case ARP_ETHERNET:
        return "Ethernet";
    default:
        break;
    }

    return "Unknown";
}

static void ip_to_string(char *str, uint32_t ip)
{
    sprintf(str, "%i.%i.%i.%i",
            ip & 0xFF,
            (ip >> 8) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 24) & 0xFF);
}

static void mac_to_string(char *str, unsigned char *mac)
{
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0],
            mac[1],
            mac[2],
            mac[3],
            mac[4],
            mac[5]);
}

static void print_arp_cache()
{
    arp_cache_entry_t *entry;

    for (int i = 0; i < ARP_CACHE_LEN; ++i)
    {
        entry = &arp_cache[i];

        if (entry->state != ARP_FREE)
        {
            char ip[32];
            char mac[32];

            ip_to_string(ip, entry->src_ip);
            mac_to_string(mac, entry->src_mac);

            log_debug("Entry %2i: hw_type=%s(0x%04x) ip=%s, mac=%s",
                      i,
                      entry->hw_type,
                      hw_type_to_string(entry->hw_type),
                      ip,
                      mac);
        }
    }
}

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
            log_debug("Inserting value into ARP cache. hw_type: %#04x",
                      header->hw_type);

            entry->state = ARP_RESOLVED;
            entry->hw_type = header->hw_type;
            entry->src_ip = data->src_ip;
            memcpy(entry->src_mac, data->src_mac, sizeof(entry->src_mac));

            print_arp_cache();

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
            log_debug("Updating value in ARP cache");
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

unsigned char *arp_cache_lookup(uint32_t ip_addr)
{
    print_arp_cache();

    arp_cache_entry_t *entry;

    for (int i = 0; i < ARP_CACHE_LEN; ++i)
    {
        entry = &arp_cache[i];

        if (entry->state == ARP_RESOLVED &&
            entry->src_ip == ip_addr)
        {
            return entry->src_mac;
        }
    }

    return NULL;
}