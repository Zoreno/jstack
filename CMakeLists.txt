cmake_minimum_required(VERSION 3.10)

project(jstack C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -Wall -Wextra -Wpedantic")

add_executable(jstack "")

target_sources(jstack PRIVATE src/device/tap.c)

target_sources(jstack PRIVATE src/ethernet/ethernet.c)

target_sources(jstack PRIVATE src/inet/parse_address.c)

target_sources(jstack PRIVATE src/endianess.c)
target_sources(jstack PRIVATE src/main.c)
target_sources(jstack PRIVATE src/utils.c)

target_include_directories(jstack PRIVATE include)
