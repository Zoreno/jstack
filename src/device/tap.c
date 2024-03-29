/**
 * @file tap.c
 * @author Joakim Bertils
 * @brief Linux kernel TAP Interface
 * @version 0.1
 * @date 2020-11-12
 * 
 * 
 */

#include "device/tap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "logging.h"
#include "utils.h"

//==============================================================================
// Definitions
//==============================================================================

#define MAX_IF_NAME_SIZE 32

//==============================================================================
// Local variables
//==============================================================================

// TODO: Move this to TAP struct
static int tap_fd;
static const char *tap_device = "/dev/net/tap";

//==============================================================================
// Local functions
//==============================================================================

// Set route for the interface.
static int set_if_route(char *dev, char *cidr)
{
    return command("ip route add dev %s %s", dev, cidr);
}

// Set an address for the interface
static int set_if_addr(char *dev, char *cidr)
{
    return command("ip address add dev %s local %s", dev, cidr);
}

// Set an address for the interface
static int set_ip_addr6(char *dev, char *cidr)
{
    return command("ip -6 address add dev %s local %s", dev, cidr);
}

// Set interface link status to up.
static int set_if_up(char *dev)
{
    return command("ip link set dev %s up", dev);
}

static int tap_alloc(char *dev)
{
    struct ifreq ifr;
    int fd;
    int error;

    fd = open(tap_device, O_RDWR);

    if (fd < 0)
    {
        log_error("Cannot open TAP device");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    // IFF_TAP: TAP Device
    // IFF_NO_PI: No extra packet information appended to the packet.
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev)
    {
        // Copy the interface name if provided.
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    error = ioctl(fd, TUNSETIFF, (void *)&ifr);

    if (error < 0)
    {
        log_error("Could not IOCTL TAP device: %s", strerror(errno));
        close(fd);
        return error;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

//==============================================================================
// Interface functions
//==============================================================================

int tap_init()
{
    char *dev = malloc(MAX_IF_NAME_SIZE);

    tap_fd = tap_alloc(dev);

    if (tap_fd < 0)
    {
        log_error("Error allocating TAP device");
        return -1;
    }

    // Set the status of the interface to UP.
    if (set_if_up(dev) != 0)
    {
        log_error("Error setting up interface");
        return -2;
    }

    // Route all traffic heading to 10.0.0.* on this interface.
    if (set_if_route(dev, "10.0.0.0/24") != 0)
    {
        log_error("Error when setting route for interface");
        return -3;
    }

    if (set_if_addr(dev, "10.0.0.5/24") != 0)
    {
        log_error("Error when setting address for interface");
        return -4;
    }

    return 0;
}

int tap_read(char *buf, int len)
{
    return read(tap_fd, buf, len);
}

int tap_write(char *buf, int len)
{
    return write(tap_fd, buf, len);
}

//==============================================================================
// End of file.
//==============================================================================
