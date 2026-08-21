#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for macos_x86_64_clang
# Host (no Docker) build using Apple clang on macOS Intel.
# @author: inx limited


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

EHS_ARCH=x86_64
EHS_OS=macos

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=darwin

EHS_TOOLCHAIN_TYPE=clang
TOOLCHAIN_NAME=HOST

# macOS host build: tells platform.mk to resolve kernel/middleware paths from ert-build-support
# using the host system's include/lib paths (analogous to EHS_HOST_DEBIAN_BUILD on Linux).
EHS_HOST_MACOS_BUILD=x86_64

COMPONENT_VARIANT=base


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

EHS_DEBUGALL=true


#################################################################################################################
# macOS Compatibility Definitions
# macOS is BSD-derived and lacks several Linux-only APIs:
#   EHS_MACOS              - enables macOS code paths in linux_ALL shared sources
#   EHS_BSD                - skips sys/timex.h include in target_time.c
#   EHS_USE_ORDINARY_NANOSLEEP - uses nanosleep() instead of clock_nanosleep() (not on macOS)
#################################################################################################################

DEFS += EHS_MACOS
DEFS += EHS_BSD
DEFS += EHS_USE_ORDINARY_NANOSLEEP
# _DARWIN_C_SOURCE: expose full macOS API surface even when individual files define
# _XOPEN_SOURCE (e.g. target_file.c defines _XOPEN_SOURCE 500 to get nftw, which on
# macOS causes cdefs.h to set _POSIX_C_SOURCE and hide snprintf in stdio.h).
DEFS += _DARWIN_C_SOURCE


#################################################################################################################
# Feature Configuration
#################################################################################################################

EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all

EHS_DEVMAN_SUPPORT=http

EHS_PERIPHERAL_DEVICE_SUPPORT=all


#################################################################################################################
# Device Management Credentials
#################################################################################################################

#DEVMAN_SERVER_DOMAIN=devman.inx-systems.com
#DEVMAN_SERVER_PROTOCOL=https

include ./target/devman-configs/inx-systems.com.mk


################################### END OF CONFIGURATION ###################################################
