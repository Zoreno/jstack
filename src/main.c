/**
 * @file main.c
 * @author Joakim Bertils
 * @version 0.1
 * @date 2020-11-22
 * 
 * @brief Main file.
 * 
 * @copyright Copyright (C) 2020,
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https: //www.gnu.org/licenses/>.
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <signal.h>
// TODO: Remove these.
#include <pthread.h>
#include <sys/types.h>

#include "arp/arp.h"
#include "device/tap.h"
#include "device/netdev.h"
#include "ip/ipv4.h"

#include "logging.h"
#include "utils.h"

#include "platform/linux/mutex.h"
#include "platform/linux/thread.h"

#define BUFFER_SIZE 128

// TODO: Investigate the need for this.
#ifndef SIG_BLOCK
#define SIG_BLOCK 1
#endif

sigset_t mask;
int running = 1;

thread_t netdev_tx_tid;
thread_t stack_handler_tid;

void handle_frame(netdev_t *netdev, eth_header_t *header)
{
    switch (header->ethertype)
    {
    case ETH_P_ARP:
        log_info("Got incoming ARP!");
        arp_incoming(netdev, header);
        break;
    case ETH_P_IP:
        ipv4_incoming(netdev, header);
        break;
    case ETH_P_IPV6:
        log_trace("Got IPv6 Packet!");
        break;
    default:
        log_warn("ETH: Unrecognized ethernet type: %#04x",
                 header->ethertype);
        break;
    }
}

static void *stop_stack_handler(void *arg)
{
    int err, signo;

    for (;;)
    {
        err = sigwait(&mask, &signo);
        if (err != 0)
        {
            log_error("Sigwait failed: %d\n", err);
        }

        switch (signo)
        {
        case SIGINT:
        case SIGQUIT:
            running = 0;
            thread_cancel(&netdev_tx_tid);
            log_info("Caught interrupt signal. Stopping stack...");
            return 0;
        default:
            log_warn("Unexpected signal %d\n", signo);
        }
    }

    return NULL;
}

static void *netdev_tx_thread(void *arg)
{
    netdev_t *tap_device = (netdev_t *)arg;

    char buffer[BUFFER_SIZE];

    while (1)
    {
        if (netdev_receive(tap_device, buffer, BUFFER_SIZE) < 0)
        {
            log_warn("ERR: Read from tun_fd: %s", strerror(errno));
        }

        eth_header_t *header = parse_eth_header(buffer);

        handle_frame(tap_device, header);
    }

    return NULL;
}

static void init_signals()
{
    int err;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);

    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
    {
        log_error("SIG_BLOCK error\n");
        exit(1);
    }
}

static void init_stack(netdev_t *tap_device)
{
    if (tap_init() < 0)
    {
        log_fatal("Failed to create TAP device");
        exit(1);
    }

    log_info("TAP device initialized");

    netdev_init(tap_device, "10.0.0.4", "00:0c:29:6d:50:25", 1500);
    tap_device->read_func = tap_read;
    tap_device->send_func = tap_write;

    arp_init();
}

static void run_threads(netdev_t *tap_device)
{
    thread_create(&netdev_tx_tid, netdev_tx_thread, tap_device);
    thread_create(&stack_handler_tid, stop_stack_handler, NULL);
}

static void join_threads()
{
    int err = thread_join(&netdev_tx_tid, NULL);

    if (err)
    {
        log_error("Failed to join netdev thread");
        exit(1);
    }
    else
    {
        log_info("Joined with netdev thread");
    }

    err = thread_join(&stack_handler_tid, NULL);

    if (err)
    {
        log_error("Failed to join stack handler thread");
        exit(1);
    }
    else
    {
        log_info("Joined with stack handler thread");
    }
}

int main()
{
    netdev_t tap_device;

    init_signals();
    init_stack(&tap_device);

    run_threads(&tap_device);
    join_threads();

    return 0;
}

//==============================================================================
// End of file.
//==============================================================================