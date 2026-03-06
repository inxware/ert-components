#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_arm64_qt_arduino-uno-q
# @author: inx limited

# arm64 Debian 13 with Qt6, targeting hardware with an Arduino UNO-Q peripheral.
# Includes OpenCV and ML support.


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/linux_arm64_debian13_base/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
# CPU architecture, operating system, toolchain selection, and middleware dependencies
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Debug and Startup Modifiers
# Debug levels, logging, console settings, and startup behavior
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Feature Configuration
# Enable/disable features, component support options, and peripheral configurations
#################################################################################################################

#----- Networking Features -----
EHS_MQTT_SUPPORT=aws_green_grass
# TODO: We need to fix the build when this isn't set.
EHS_DEVMAN_SUPPORT=http

#----- GUI Features -----
EHS_MAIN_LOOP_ITERATIVE=yes
EHS_GUI_SUPPORT=qt
EHS_GUI_SUPPORT_QT6=yes

#----- Machine Vision / ML Features -----
EHS_ML_SUPPORT=yes
EHS_ML_FRAMEWORK_IMAGE_SUPPORT=tensorflow-lite
EHS_ML_HARDWARE_ACCELERATION=none

EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET=yes
EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET=yes
EHS_MV_SUPPORT=opencv
#EHS_MV_SUPPORT=stubbed

# Use libcamera on top of opencv if supported
#EHS_USE_LIBCAMERA=yes

#----- Peripheral Features -----
# (Inherits from parent config)


#################################################################################################################
# Application and Packaging
# Default application, system variant, and packaging/deployment options
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Device Management Credentials
# Include files containing server connection details and credentials
#################################################################################################################

# (No devman config for this target)


#################################################################################################################
# Legacy Preprocessor Definitions (DEFS)
# Direct preprocessor definitions - should be migrated to proper make variables where possible
#################################################################################################################

# (No legacy DEFS for this target)


################################### END OF CONFIGURATION ###################################################
