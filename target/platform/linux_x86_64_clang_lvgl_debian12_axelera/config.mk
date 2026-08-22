#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# @author: inx limited

# Usage Description
# x86_64 Linux (Debian 12) with LVGL, targeting machines fitted with an
# Axelera Metis NPU (PCIe).  libaxruntime.so is loaded at runtime via
# dlopen — the binary runs on machines without the card installed and
# logs a clear error at model-load time instead of refusing to start.
#
# Mirrors linux_x86_64_clang_lvgl_debian13_axelera but on the Debian 12
# baseline (clang 14 toolchain, bookworm runtime libs).  The Axelera
# Voyager SDK packages come from the same public APT repository; the
# distribution string ubuntu24 main works for both Debian 12 and 13
# baselines (the runtime is dlopen'd and the headers are libc-version
# tolerant).


#################################################################################################################
# Baseline Configuration Includes
#################################################################################################################

include ./target/platform/linux_x86_64_clang_lvgl_gg_debian12/config.mk


#################################################################################################################
# Architecture, OS, Toolchain and Dependencies
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Debug and Startup Modifiers
#################################################################################################################

# (Inherits from parent config)


#################################################################################################################
# Feature Configuration
#################################################################################################################

#----- Machine Learning — Axelera Metis NPU -----
# Override the parent's TFLite CPU baseline with Axelera NPU acceleration.
# EHS_ML_IE_IMAGE_SUPPORT intentionally set to none — no TFLite CPU fallback;
# the Metis NPU handles all inference.
EHS_ML_SUPPORT=yes
EHS_ML_HARDWARE_ACCELERATION=axelera
EHS_ML_IE_IMAGE_SUPPORT=none


#################################################################################################################
# Application and Packaging
#################################################################################################################

# Augment the parent's runtime package list with Axelera Voyager SDK packages.
# The base Debian 12 runtime libs are already pulled in by the parent.
# Axelera packages require the Axelera APT repo configured on the target machine.
DEBIAN_PACKAGE_PLATFORM_EXTRA += \
    axelera-runtime-1.6.0 \
    axelera-device-1.6.0


#################################################################################################################
# Device Management Credentials
#################################################################################################################

# (No devman config — no-certs variant inherited from parent)

################################### END OF CONFIGURATION ###################################################
