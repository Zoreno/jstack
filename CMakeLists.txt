cmake_minimum_required(VERSION 3.10)

project(jstack C)

set(PLATFORM "linux")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -Wall -Wextra -Wpedantic")

add_executable(jstack "")

target_sources(jstack PRIVATE src/arp/arp.c)
target_sources(jstack PRIVATE src/arp/arp_input.c)
target_sources(jstack PRIVATE src/arp/arp_output.c)
target_sources(jstack PRIVATE src/arp/arp_cache.c)

target_sources(jstack PRIVATE src/device/tap.c)
target_sources(jstack PRIVATE src/device/netdev.c)

target_sources(jstack PRIVATE src/ethernet/ethernet.c)

target_sources(jstack PRIVATE src/icmp/icmpv4_checksum.c)
target_sources(jstack PRIVATE src/icmp/icmpv4_input.c)
target_sources(jstack PRIVATE src/icmp/icmpv4_output.c)

target_sources(jstack PRIVATE src/inet/parse_address.c)

target_sources(jstack PRIVATE src/ip/ipv4_checksum.c)
target_sources(jstack PRIVATE src/ip/ipv4_debug.c)
target_sources(jstack PRIVATE src/ip/ipv4_input.c)
target_sources(jstack PRIVATE src/ip/ipv4_output.c)

if (${PLATFORM} STREQUAL "linux")
include(linux)
endif()

target_sources(jstack PRIVATE src/endianess.c)
target_sources(jstack PRIVATE src/list.c)
target_sources(jstack PRIVATE src/logging.c)
target_sources(jstack PRIVATE src/mac_address.c)
target_sources(jstack PRIVATE src/main.c)
target_sources(jstack PRIVATE src/timer.c)
target_sources(jstack PRIVATE src/utils.c)

target_include_directories(jstack PRIVATE include)

add_custom_target(run
    COMMAND sudo ./jstack
    DEPENDS jstack
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build
    COMMENT "Running the netstack")