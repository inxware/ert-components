#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_qt_debian12-no-certs
# @author: inx limited

# Raspberry Pi (4B or 5) with Qt6, Debian 12.
# We're assuming Pi 4B or 5 below. Other flavours may work but haven't been tested.


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
EHS_ARCH=arm64
EHS_OS=linux
EHS_GNU_ARCH=arm64
EHS_GNU_OS=linux-gnu

# This determines the /usr/<arch> when using the host's toolchain
EHS_HOST_DEBIAN_BUILD=arm64

# This will indicate what ert-contrib-middleware is used and toolchains if not using a host toolchain
# WARNING: Using greengrass built for debian 11 in ert-contrib-middleware
EHS_TARGET_LIB_VARIANT=-clang11_debian11

# Configure some library version choices and Debian packager specifics
EHS_DEBIAN_VERSION=12

# Use host toolchain + Qt headers and static objects from Docker
TOOLCHAIN_NAME=HOST

# Select the os-arch directory with these
EHS_TOOLCHAIN_TYPE=clang
LINK_OVERRIDE=clang

# SYSTEM_VARIANT=RASPBERRYPI - uses libexptw when enabled; disabled pending investigation
##SYSTEM_VARIANT=RASPBERRYPI

# Contributed library dependencies variant
COMPONENT_VARIANT=base


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# Debug/Production mode
EHS_DEBUGALL=true
#EHS_DEBUG_TCPIP_CONSOLE=stubbed
EHS_DEBUG_TCPIP_CONSOLE=yes

# Graphics subsystem log level
#EHS_LOG_LEVEL_GRAPHICS=EHSH_LOG_LEVEL_ALL


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

# eRT1 SODL support
ERT_SODL_VERSION=1

#----- Networking Features -----
EHS_NETWORKING_SUPPORT=all
EHS_COMPONENT_NETWORKING_SUPPORT=all
EHS_MQTT_SUPPORT=aws_green_grass

# Unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
# Qt uses iterative event loop pattern in `target_main.c`. This means we poll `EhsMainLoop()` from a Qt
# timer to enable EHS to progress its state, rather than the more traditional (blocking) call into `EhsLoop()`.
EHS_MAIN_LOOP_ITERATIVE=yes

EHS_GUI_SUPPORT=qt
# Set this as a modifier for Qt - not a whole new way of working.
EHS_GUI_SUPPORT_QT6=yes
#TODO2026 - we probably want Qt6 to be the default and the extra flag would be for Qt5.

# This target's binary also runs on Boot2Qt images (b2qt-raspberrypi-armv8), not just
# Debian 12.  Rendering there is eglfs + eglfs_kms + GBM straight onto DRM/KMS - no X11,
# no compositor - with v3d rendering and a separate display-only DRM device for scanout
# (vc4-drm for HDMI, drm-rp1-dsi for an MIPI panel on a Pi 5).
#
# Driving an MIPI/DSI panel instead of HDMI needs target-side runtime config that this
# build does NOT set: display_auto_detect in /boot/config.txt and the DRM device in
# /etc/kms.conf.  Get either wrong and HDMI silently keeps working, so nothing looks
# broken.  See boot2qt-display-config.md in this directory for the full rendering stack,
# the exact settings, and how to verify which screen Qt picked.
#
# That document also covers adding eRT to the Boot2Qt launcher as a tile (with the
# ready-made descriptor, launch wrapper and branded icon in boot2qt-launcher/), and
# hiding the mouse pointer.  Note a tile runs the app as a WAYLAND CLIENT inside the
# launcher's compositor, not on eglfs - read the "Two traps" subsection before wiring
# one up, it is not just a matter of adding an XML file.

#EHS_AV_SUPPORT=devmanonly
EHS_MEDIA_SUPPORT=all

# To enable AV media support set EHS_AV_SUPPORT to {gst,vlc} depending on target support.


#----- Machine Vision / ML Features -----
# Both are OFF for this target.  The valid alternatives are spelled out below so they
# don't have to be reverse-engineered from the HAL directories each time.
#
# EHS_MV_SUPPORT       - machine vision backend.  One of:
#                          stubbed | opencv | none      (also jetson | android, other platforms)
#                        Backends are the directories in target/Component-HAL/mv/.
# EHS_USE_LIBCAMERA    - yes.  Only meaningful with opencv; needed for the Pi CSI camera
#                        (a USB/UVC webcam works through opencv without it).
# EHS_ML_SUPPORT       - yes, to build the ML HAL at all.
# EHS_ML_IE_IMAGE_SUPPORT
#                      - inference engine.  One of:
#                          none | tensorflow-lite | tensorflow-lite-micro
#                        Use tensorflow-lite here; -micro is for MCU targets.
# EHS_ML_HARDWARE_ACCELERATION
#                      - one of: none | hailo | axelera | nvidia
#                        For a Pi: none (CPU) or hailo (Pi AI Kit / Hailo-8L).
#                        axelera = Axelera M.2, nvidia = Jetson - neither applies here.
# EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET / _YOLOV8_OBJDET
#                      - yes, to build the matching post-processing.  Needs EHS_ML_SUPPORT.
#
# See docs/llm-dev-contexts/CLAUDE-ml-hal.md for the EhsML_* API and the tf_lite_frame block.
#
# Typical CPU-inference setup for this target (mirrors linux_arm64_lvgl_raspberrypi_debian12):
#   EHS_MV_SUPPORT=opencv
#   EHS_USE_LIBCAMERA=yes
#   EHS_ML_SUPPORT=yes
#   EHS_ML_IE_IMAGE_SUPPORT=tensorflow-lite
#   EHS_ML_HARDWARE_ACCELERATION=none
#   EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
#   EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
#
# For Hailo acceleration, ALSO rebuild the Docker image with HailoRT - it is gated off by
# default because the .deb is not in the repo:
#   drop hailort_<ver>_arm64.deb into <repo-root>/temp/, then
#   ERT_DOCKER_BUILD_ARGS=INSTALL_HAILO=1 make build_docker_local
# and set EHS_ML_HARDWARE_ACCELERATION=hailo with EHS_ML_IE_IMAGE_SUPPORT=none
# (see linux_arm64_lvgl_raspberrypi_demo_hailo for a worked example).
EHS_MV_SUPPORT=stubbed

#----- Peripheral Features -----
# NOTE: exactly ONE uncommented assignment per variable.  This file used to set several
# of these twice and make silently took the last one, so the visible value was not the
# effective one.  Alternatives are listed commented-out above each live setting.
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# GPIO.  Alternatives: sferalabs | sysfs_linux_arm | pigpio | wiringpi | stubbed
#   sysfs_linux_arm - no library dependency, works on Pi 4 and Pi 5
#   wiringpi        - Pi 5 capable (WiringPi 3.x); needs libwiringPi in the Docker image
#   pigpio          - Pi 1-4 ONLY.  Bit-bangs BCM registers; the Pi 5 moved GPIO behind
#                     the RP1 southbridge, so pigpio does not work there at all.
EHS_PERIPHERALS_GPIO_SUPPORT=stubbed

# ADC/DAC.  Alternatives: sferalabs | SPI_A6_LTC241X | stubbed
EHS_PERIPHERALS_ADC_DAC_SUPPORT=stubbed

# PWM.  Alternatives: pigpio | wiringpi | stubbed   (same Pi 5 caveat as GPIO above)
EHS_PERIPHERALS_PWM_SUPPORT=stubbed

#################################################################################################################
# Sfera Labs Peripheral HAL Support
# Enable the new peripheral HAL blocks for the Strato Pi Max (and other Sfera Labs boards where available).
#
# DISABLED - this target is a plain Raspberry Pi 4B/5 (currently a Pi 5 + Boot2Qt + MIPI
# DSI panel), not a Sfera Labs board, so none of these HALs have hardware to talk to.
# The ADC/DAC line below also used to silently override the stubbed setting above.
# Uncomment the block as a whole if this target is ever pointed at a Strato Pi.
#################################################################################################################

#EHS_PERIPHERALS_ADC_DAC_SUPPORT=sferalabs
#EHS_WATCHDOG_SUPPORT=sferalabs
#EHS_UPS_SUPPORT=sferalabs
#EHS_BUZZER_SUPPORT=sferalabs
#EHS_SD_SELECT_SUPPORT=sferalabs
#EHS_USB_POWER_SUPPORT=sferalabs
#EHS_PERIPHERALS_ACCEL_GYRO_SUPPORT=sferalabs
#EHS_RS485_CONFIG_SUPPORT=sferalabs

#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# Application Selection
EHS_DEFAULT_APP=customer-apps/qt/hello_world-qt
#EHS_DEFAULT_APP=demos/QT_UIs/Particles
#EHS_DEFAULT_APP=demos/simple-qt-socket_webserver

EHS_PACKAGER_TYPE=deb

DEBIAN_PACKAGE_PLATFORM_EXTRA= \
qml6-module-qtquick \
qml6-module-qtquick-timeline \
qt6-base-dev qt6-declarative-dev \
qml6-module-qtquick \
qml6-module-qtquick-timeline \
qml6-module-qtquick-templates \
qml6-module-qtquick-window \
qml6-module-qtquick3d \
qml6-module-quick3d-particles3d \
qml6-module-quick3d-helpers \
qml6-module-quick3d-effects \
qml6-module-qtquick-window \
qml6-module-qtquick-controls \
qml6-module-qtquick-layouts \
qml6-module-qtquick-templates \
qml6-module-qtqml-workerscript


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target - no-certs variant)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# We should try Wi-Fi by default in Raspberry Pi devices
DEFS += EHS_USE_WIFI_INTERFACE=1
DEFS += EHS_DEBIAN_VERSION=12

DEFS += EHS_LOG_LEVEL_VERBOSE
DEFS += EHS_LOG_TO_STDIO

# Force 'unknown' source files to output logging at the logger's logging level
DEFS += EHSL_MODULE_ID=EHSH_LOG_MODULE_LOGGER

# Enable QML singleton scanning support in the object table builder.
# When defined, ertqt_add_singleton_scan() is compiled in and callers can register QML expressions
# (e.g. "Rooms") to be evaluated via QQmlExpression so that objects inside pragma Singleton files
# appear in the object table. Requires QQmlExpression / QQmlContext headers (included automatically).
DEFS += ERTQT_SINGLETON_SCAN

# EHS_MOCK_GPIO_QT: Mock GPIO widgets in Qt UI for desktop development before hardware is available.
# EHS_MOCK_GPIO_QT=yes
# DEFS += EHS_MOCK_GPIO_QT

# Enables a floating 'debug_cam_X' window with camera preview
#DEFS+=EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW=1


# LoRaWAN modem support (WIO-E5 via UART/serial)
EHS_LORAWAN_SUPPORT=wio_e5

################################### END OF CONFIGURATION ###################################################
