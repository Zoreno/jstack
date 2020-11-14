#include "arp/arp.h"

#include "arp/arp_cache.h"

#include "logging.h"

void arp_init()
{
    log_info("Initalizing ARP module");

    arp_cache_init();
}