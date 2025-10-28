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
# Set general architecture and OS version 
#################################################################################################################


# ehs is more generic and doesn't use special libc magic.
SYSTEM_VARIANT=windesktop
EHS_ARCH=x86#
EHS_OS=mingw
TOOLCHAIN_NAME=HOST
EHS_GNU_ARCH=i686
EHS_GNU_OS=mingw32posix
CC_OVERRIDE=i686-w64-mingw32-gcc-10-posix 

# Used for specifing name of the eRT package/executable
ERT_PACKAGE_NAME=ehs
# Used for specifing user facing name of installed application (windows installer)
ERT_NSIS_EXE_NAME=eRT

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
# Some debug options
#EHS_DEBUGALL=true
EHS_DEBUG_TCPIP_CONSOLE=yes

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

EHS_NETWORKING_SUPPORT=all# This needs to be enabled for the core EHS - there is a HAL dependency
#@todo we need an entry here for the URL components etc.
EHS_COMPONENT_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

EHS_MQTT_SUPPORT=aws_green_grass
################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
#COMPONENT_VARIANT is the postfix after archicture identifiers to define a specific set of components
#Note - windows targets in componentlibrary use hyphens between components (randomly)
COMPONENT_VARIANT=sdl2-ffmpeg

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=lvgl
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=ffmpeg
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
# Keyboard and stuff..
EHS_PERIPHERAL_DEVICE_SUPPORT=all
#emulate GPIO for windows:
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed
EHS_PID_SUPPORT=gnu
EHS_SCHEDULER_SUPPORT=1
EHS_MODBUS_SUPPORT=stubbed
################################### END OF TOOLBOX CONFIGURATION ###################################################


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
#
DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE=none
DEVMAN_SERVER_CERTS_CLIENT_AUTH_REQUIRED=no