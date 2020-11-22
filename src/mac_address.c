#include "mac_address.h"

#include <string.h>
#include <stdio.h>

void mac_address_clear(mac_address_t *mac)
{
    memset(mac->octets, 0, MAC_ADDRESS_OCTET_COUNT);
}

void mac_address_broadcast(mac_address_t *mac)
{
    memset(mac->octets, 0xFF, MAC_ADDRESS_OCTET_COUNT);
}

void mac_address_copy(mac_address_t *dst, const mac_address_t *src)
{
    memcpy(dst->octets, src->octets, MAC_ADDRESS_OCTET_COUNT);
}

int mac_address_equal(const mac_address_t *mac1, const mac_address_t *mac2)
{
    return memcmp(mac1->octets, mac2->octets, MAC_ADDRESS_OCTET_COUNT);
}

void mac_address_to_string(char *str, const mac_address_t *mac)
{
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac->octets[0],
            mac->octets[1],
            mac->octets[2],
            mac->octets[3],
            mac->octets[4],
            mac->octets[5]);
}

void mac_address_from_string(mac_address_t *mac, const char *str)
{
    int values[MAC_ADDRESS_OCTET_COUNT];

    int parsed_vals = sscanf(str, "%i:%i:%i:%i:%i:%i",
                             &values[0],
                             &values[1],
                             &values[2],
                             &values[3],
                             &values[4],
                             &values[5]);

    if (parsed_vals != 6)
    {
        mac_address_clear(mac);
        return;
    }

    for (int i = 0; i < MAC_ADDRESS_OCTET_COUNT; ++i)
    {
        mac->octets[i] = (uint8_t)values[i];
    }
}