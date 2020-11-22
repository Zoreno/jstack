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
#include "ethernet/ethernet.h"
#include "ip/ipv4.h"
#include "platform/linux/mutex.h"
#include "platform/linux/thread.h"

#include "list.h"
#include "logging.h"
#include "utils.h"

// TODO: Investigate the need for this.
#ifndef SIG_BLOCK
#define SIG_BLOCK 1
#endif

sigset_t mask;
int running = 1;

list_t *threads;

static void cancel_all_other_threads()
{
    list_node_t *node;
    thread_t self = thread_self();

    for (node = threads->head; node != NULL; node = node->next)
    {
        thread_t *thread = (thread_t *)node->payload;

        if (!thread_equal(&self, thread))
        {
            thread_cancel(thread);
        }
    }
}

static void *stop_stack_handler(void *arg)
{
    int err, signo;
    (void)arg;

    while (1)
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
            cancel_all_other_threads();
            log_info("Caught interrupt signal. Stopping stack...");
            return 0;
        default:
            log_warn("Unexpected signal %d\n", signo);
        }
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
    thread_t *netdev_rx_tid = malloc(sizeof(thread_t));
    thread_t *stack_handler_tid = malloc(sizeof(thread_t));

    thread_create("Netdev RX", netdev_rx_tid, netdev_rx_thread, tap_device);
    list_insert(threads, netdev_rx_tid);

    thread_create("Stack Handler", stack_handler_tid, stop_stack_handler, NULL);
    list_insert(threads, stack_handler_tid);
}

static void join_threads()
{
    list_node_t *node;

    while ((node = list_pop(threads)) != NULL)
    {
        thread_t *thread = (thread_t *)node->payload;

        int err = thread_join(thread, NULL);

        if (err)
        {
            log_error("Failed to join thread '%s'", thread->name);
            exit(1);
        }
        else
        {
            log_info("Joined with thread '%s'", thread->name);
        }
    }
}

int main()
{
    threads = list_create();

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