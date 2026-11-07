# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stage9_command_tests)
set(STAGE9 ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_sources(app PRIVATE src/main.c ${STAGE9}/src/commands.c
  ${STAGE9}/src/memory_commands.c ${STAGE9}/src/register_commands.c
  ${STAGE9}/third_party/ntshell/util/ntopt.c)
target_include_directories(app PRIVATE ${STAGE9}/src
  ${STAGE9}/third_party/ntshell/core ${STAGE9}/third_party/ntshell/util)
