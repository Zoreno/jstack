#ifndef _MAC_ADDRESS_H
#define _MAC_ADDRESS_H

#include <stdint.h>

#define MAC_ADDRESS_OCTET_COUNT 6

typedef struct _mac_address
{
    uint8_t octets[MAC_ADDRESS_OCTET_COUNT];
} __attribute__((packed)) mac_address_t;

void mac_address_clear(mac_address_t *mac);
void mac_address_broadcast(mac_address_t *mac);
void mac_address_copy(mac_address_t *dst, const mac_address_t *src);
int mac_address_equal(const mac_address_t *mac1, const mac_address_t *mac2);
void mac_address_to_string(char *str, const mac_address_t *mac);
void mac_address_from_string(mac_address_t *mac, const char *str);

#endif