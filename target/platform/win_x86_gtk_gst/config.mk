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

EHS_GNU_ARCH=i686
EHS_GNU_OS=mingw32posix

TOOLCHAIN_NAME=HOST
CC_OVERRIDE=i686-w64-mingw32-gcc-10-posix

SYSTEM_VARIANT=windesktop

# Contributed library dependencies variant
COMPONENT_VARIANT=gtk-gst


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

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_GUI_SUPPORT=gtk
#EHS_AV_SUPPORT=gst - not with i686 mingw
EHS_AV_SUPPORT=gst10
EHS_VIDEO_SUPPORT=yes
EHS_MEDIA_SUPPORT=all

#----- Peripheral Features -----
EHS_PERIPHERAL_DEVICE_SUPPORT=all
EHS_PERIPHERALS_GPIO_SUPPORT=gui
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed

#EHS_MQTT_SUPPORT=aws_green_grass @TODO - disabling this for now, as it's causing build error


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

#The following runtime dependencies must be found in the ../ert-contrib-middleware/target_libs/.../build/libs directory
#
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-glib2-2.74.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-gtk3-3.24.34-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-curl-7.85.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-pango-1.50.9-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-harfbuzz-5.2.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-cairo-1.17.6-3-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-gdk-pixbuf2-2.42.9-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-atk-2.38.0-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-gstreamer-1.20.3-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libarchive-3.6.1-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libxml2-2.9.14-4-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libiconv-1.17-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-expat-2.4.9-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-zlib-1.2.12-1-any.pkg.tar.zst

#The following runtime dependencies must be found in the ../ert-contrib-middleware/target_libs/.../target_runtime/ directory

#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libb2-0.98.1-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-openssl-1.1.1.q-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libb2-0.98.1-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-lz4-1.9.4-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libwinpthread-git-10.0.0.r83.gad45df377-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-fontconfig-2.14.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-freetype-2.12.1-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-pixman-0.40.0-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-zstd-1.5.2-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libpsl-0.21.1-4-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-zlib-1.2.12-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libidn2-2.3.3-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-nghttp2-1.48.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libssh2-1.9.0-5-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-brotli-1.0.9-5-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libpng-1.6.38-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libepoxy-1.5.9-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-fribidi-1.0.12-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-pcre2-10.40-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libffi-3.4.3-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-graphite2-1.3.14-2-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libthai-0.1.29-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libunistring-1.0-1-any.pkg.tar.zst
#https://repo.msys2.org/mingw/mingw32/mingw-w64-i686-libdatrie-0.2.13-1-any.pkg.tar.zst

#https://download.zip.dll-files.com/ebf7e3a538843562562fb3994adf5c9c/libbz2-1.zip?token=bs9IE2uw4c7GTjLCNkLkDg&expires=1664950481
#https://www.dllme.com/getfile.php?file=27548&id=14349a7d3b45e32cab4a172da41c0273
#https://osdn.net/projects/mingw/storage/mingw32-packages/contributed/liblzma/liblzma-5.2.5/liblzma-5.2.5-2-mingw32-dll-5.tar.xz/
#https://waterlan.home.xs4all.nl/libintl/libintl-relocatex-0.18.1.1-2RX-mingw32-dll-8.zip

#The following DLLS are also required and are founf in the host's mingw usr/lib:
#/usr/lib/gcc/i686-w64-mingw32/10-posix/libgcc_s_dw2-1.dll
#/usr/lib/gcc/i686-w64-mingw32/10-win32/libstdc++-6.dll


################################### END OF CONFIGURATION ###################################################
