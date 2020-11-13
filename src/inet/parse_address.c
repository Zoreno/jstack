/**
 * @file parse_address.c
 * @author Joakim Bertils
 * @brief Parse IP addresses.
 * @version 0.1
 * @date 2020-11-12
 * 
 * Routines for parsing IPv4 and IPv6 Addresses into binary form.
 */

#include "inet/parse_address.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define _AF_INET 2
#define _AF_INET6 10

#define NS_INADDRSZ 4
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ 2

//==============================================================================
// Forward declarations
//==============================================================================

static int parse_address4(const char *src, const char *src_end, uint8_t *dst);
static int parse_address6(const char *src, const char *src_end, uint8_t *dst);
static int parse_address_len(int af, const char *src, size_t srclen, void *dst);
static int hex_digit_value(char ch);

//==============================================================================
// Local variables
//==============================================================================

//==============================================================================
// Local functions
//==============================================================================

static int parse_address4(const char *src, const char *src_end, uint8_t *dst)
{
    int saw_digit = 0;
    int octets = 0;
    int ch;
    unsigned char tmp[NS_INADDRSZ];
    memset(tmp, 0, sizeof(tmp));
    unsigned char *tp = tmp;

    while (src < src_end)
    {
        ch = *src++;

        if (ch >= '0' && ch <= '9')
        {
            unsigned int new = *tp * 10 + (ch - '0');

            if (saw_digit && *tp == 0)
            {
                return 0;
            }

            if (new > 255)
            {
                return 0;
            }

            *tp = new;

            if (!saw_digit)
            {
                if (++octets > 4)
                {
                    return 0;
                }

                saw_digit = 1;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
            {
                return 0;
            }

            *++tp = 0;
            saw_digit = 0;
        }
        else
        {
            return 0;
        }
    }

    if (octets < 4)
    {
        return 0;
    }

    memcpy(dst, tmp, NS_INADDRSZ);
    return 1;
}

static int parse_address6(const char *src, const char *src_end, uint8_t *dst)
{
    unsigned char tmp[NS_IN6ADDRSZ];
    unsigned char *tp = tmp;
    unsigned char *endp = tp + NS_IN6ADDRSZ;
    unsigned char *colon_p = NULL;
    int ch;
    size_t hex_digits_seen = 0;
    unsigned int val = 0;
    memset(tmp, '\0', NS_IN6ADDRSZ);

    if (src == src_end)
    {
        return 0;
    }

    if (*src == ':')
    {
        ++src;
        if (src == src_end || *src != ':')
        {
            return 0;
        }
    }

    const char *curtok = src;

    while (src < src_end)
    {
        ch = *src++;
        int digit = hex_digit_value(ch);

        if (digit >= 0)
        {
            if (hex_digits_seen == 4)
            {
                return 0;
            }

            val <<= 4;
            val |= digit;

            if (val > 0xffff)
            {
                return 0;
            }

            ++hex_digits_seen;
            continue;
        }
        if (ch == ':')
        {
            curtok = src;

            if (hex_digits_seen == 0)
            {
                if (colon_p)
                {
                    return 0;
                }

                colon_p = tp;
                continue;
            }
            else if (src == src_end)
            {
                return 0;
            }

            if (tp + NS_INT16SZ > endp)
            {
                return 0;
            }

            *tp++ = (unsigned char)(val >> 8) & 0xff;
            *tp++ = (unsigned char)val & 0xff;
            hex_digits_seen = 0;
            val = 0;
            continue;
        }
        if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) && parse_address4(curtok, src_end, tp) > 0)
        {
            tp += NS_INADDRSZ;
            hex_digits_seen = 0;
            break;
        }
        return 0;
    }

    if (hex_digits_seen > 0)
    {
        if (tp + NS_INT16SZ > endp)
        {
            return 0;
        }

        *tp++ = (unsigned char)(val >> 8) & 0xff;
        *tp++ = (unsigned char)val & 0xff;
    }

    if (colon_p != NULL)
    {

        if (tp == endp)
        {
            return 0;
        }

        size_t n = tp - colon_p;
        memmove(endp - n, colon_p, n);
        memset(colon_p, 0, endp - n - colon_p);
        tp = endp;
    }

    if (tp != endp)
    {
        return 0;
    }

    memcpy(dst, tmp, NS_IN6ADDRSZ);

    return 1;
}

static int parse_address_len(int af, const char *src, size_t srclen, void *dst)
{
    switch (af)
    {
    case _AF_INET:
        return parse_address4(src, src + srclen, dst);
        break;
    case _AF_INET6:
        return parse_address6(src, src + srclen, dst);
        break;
    default:
        return -1;
        break;
    }
}

static int hex_digit_value(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }

    if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a';
    }

    if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A';
    }

    return -1;
}

//==============================================================================
// Interface functions
//==============================================================================

int parse_address(int af, const char *src, void *dst)
{
    return parse_address_len(af, src, strlen(src), dst);
}

//==============================================================================
// End of file
//==============================================================================
