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

#include "utils.h"

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
        fprintf(stderr, "Cannot open TAP device\n");
        exit(1);
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
        fprintf(stderr, "Could not IOCTL TAP device: %s\n", strerror(errno));
        close(fd);
        return error;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

//==============================================================================
// Interface functions
//==============================================================================

int tap_init(char *dev)
{
    tap_fd = tap_alloc(dev);

    if (tap_fd < 0)
    {
        fprintf(stderr, "Error allocating TAP device");
        return -1;
    }

    if (set_if_up(dev) != 0)
    {
        fprintf(stderr, "Error setting up interface\n");
        return -2;
    }

    if (set_if_route(dev, "192.168.52.0/24") != 0)
    {
        fprintf(stderr, "Error when setting route for interface\n");
        return -3;
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
