#include <stdlib.h>
#include <string.h>

#include "device/tap.h"

#include "utils.h"

#define BUFFER_SIZE 128

int main()
{
    char buffer[BUFFER_SIZE];
    char *dev = calloc(10, 1);

    memset(buffer, 0, sizeof(buffer));

    tap_init(dev);
}