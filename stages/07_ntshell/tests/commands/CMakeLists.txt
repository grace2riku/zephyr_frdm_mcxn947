# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stage7_command_tests)
set(STAGE7 ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_sources(app PRIVATE src/main.c ${STAGE7}/src/commands.c
  ${STAGE7}/third_party/ntshell/util/ntopt.c)
target_include_directories(app PRIVATE ${STAGE7}/src
  ${STAGE7}/third_party/ntshell/core ${STAGE7}/third_party/ntshell/util)
