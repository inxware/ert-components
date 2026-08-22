#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for win_x86_gtk_gst
# @author: inx limited


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

# (No parent config - this is a base configuration)


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# CPU and OS Type Selection
EHS_ARCH=x86#
EHS_OS=mingw

EHS_GNU_ARCH=x86_64
EHS_GNU_OS=mingw32posix

TOOLCHAIN_NAME=HOST
# Unversioned on purpose: the gcc version belongs to the Docker image
# (Dockerimagename), not to this config. Both jammy (gcc 10) and noble
# (gcc 13) provide this driver name, so the image can move without
# touching seven platform configs.
CC_OVERRIDE=x86_64-w64-mingw32-gcc-posix
# Must be set too: target/os-arch/mingw-x86/toolchain.mk defaults LINK_OVERRIDE
# to i686-w64-mingw32-gcc, so overriding only CC compiles 64-bit objects and
# then links them with the 32-bit linker.
LINK_OVERRIDE=x86_64-w64-mingw32-gcc-posix

SYSTEM_VARIANT=windesktop

# Contributed library dependencies variant
COMPONENT_VARIANT=sdl2-ffmpeg


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
#EHS_DEBUGALL=true
EHS_DEBUG_TCPIP_CONSOLE=yes


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=lvgl
EHS_AV_SUPPORT=ffmpeg
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
#emulate GPIO for windows:
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

#The following build dependencies must be found in the ../ert-contrib-middleware/target_libs/.../build/libs directory
#
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-SDL2-2.28.3-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-curl-8.2.1-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libxml2-2.11.5-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libiconv-1.17-3-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libarchive-3.7.2-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-expat-2.5.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-zlib-1.3-1-any.pkg.tar.zst
#
#
#The following runtime dependencies must be found in the ../ert-contrib-middleware/target_libs/.../build/libs directory
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libb2-0.98.1-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-brotli-1.1.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-bzip2-1.0.8-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-openssl-3.1.2-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-gcc-libs-13.2.0-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-zstd-1.5.5-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-lz4-1.9.4-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-xz-5.4.4-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libidn2-2.3.4-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-nghttp2-1.56.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libpsl-0.21.2-4-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-openssl-3.1.2-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libssh2-1.11.0-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libunistring-1.1-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libwinpthread-git-11.0.0.r18.g9df2e604d-1-any.pkg.tar.zst

#https://waterlan.home.xs4all.nl/libintl/libintl-relocatex-0.18.1.1-2RX-mingw32-dll-8.zip

#aws
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-cal-0.6.1-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-common-0.9.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-compression-0.2.17-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-http-0.7.11-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-io-0.13.30-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-aws-c-mqtt-0.8.14-1-any.pkg.tar.zst


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
