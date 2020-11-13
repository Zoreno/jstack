#include <stdlib.h>
#include <string.h>

#include "device/tap.h"
#include "device/netdev.h"

#include "utils.h"

#define BUFFER_SIZE 128

int main()
{
    char buffer[BUFFER_SIZE];
    char *dev = calloc(10, 1);

    netdev_t netdev;

    memset(buffer, 0, sizeof(buffer));

    tap_init(dev);

    netdev_init(&netdev, "10.0.0.4", "00:0c:29:6d:50:25");
}