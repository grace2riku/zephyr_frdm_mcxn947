# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stage8_command_tests)
set(STAGE8 ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_sources(app PRIVATE src/main.c ${STAGE8}/src/commands.c
  ${STAGE8}/src/memory_commands.c ${STAGE8}/third_party/ntshell/util/ntopt.c)
target_include_directories(app PRIVATE ${STAGE8}/src
  ${STAGE8}/third_party/ntshell/core ${STAGE8}/third_party/ntshell/util)
