# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(stage8_memory_ram)

target_sources(app PRIVATE
  src/main.c src/serial_port.c src/commands.c src/led_control.c src/memory_commands.c
  third_party/ntshell/core/ntshell.c
  third_party/ntshell/core/ntlibc.c
  third_party/ntshell/core/text_editor.c
  third_party/ntshell/core/text_history.c
  third_party/ntshell/core/vtrecv.c
  third_party/ntshell/core/vtsend.c
  third_party/ntshell/util/ntopt.c
)
target_include_directories(app PRIVATE src
  third_party/ntshell/core third_party/ntshell/util)
