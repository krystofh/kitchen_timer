#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app LANGUAGES C)
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/event_handler.c)
target_sources(app PRIVATE src/display_driver.c)
target_sources(app PRIVATE src/sound_player.c)
target_sources(app PRIVATE src/timer.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
