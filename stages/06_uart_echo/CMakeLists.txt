# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(ECHO_MODE "byte" CACHE STRING "Echo lesson: byte, line or irq")
set_property(CACHE ECHO_MODE PROPERTY STRINGS byte line irq)

if(ECHO_MODE STREQUAL "irq")
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/prj_irq.conf")
else()
  set(CONF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/prj.conf")
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(uart_echo)

if(ECHO_MODE STREQUAL "byte")
  target_sources(app PRIVATE src/main.c)
elseif(ECHO_MODE STREQUAL "line")
  target_sources(app PRIVATE src/poll_line.c)
elseif(ECHO_MODE STREQUAL "irq")
  target_sources(app PRIVATE src/irq_line.c)
else()
  message(FATAL_ERROR "ECHO_MODE must be byte, line or irq")
endif()

message(STATUS "Stage 6 ECHO_MODE=${ECHO_MODE}")
