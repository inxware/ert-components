#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk
# inxware ERT configuration file for linux_android_arm_legacy
# @author: inx limited

# Base configuration for fixed/embedded Android hardware targets that cannot
# guarantee Play Store minSdkVersion requirements.  Sets EHS_ANDROID_MIN_SDK=16
# so the APK installs on any Android device the hardware may ship with.
#
# Hardware-specific platforms (p64_a6, p64_h6, rock64, rk3566, etc.) include
# this file as their parent.  For Play Store or modern-hardware-only deployments
# use linux_android_arm instead (minSdkVersion 23).


#################################################################################################################
# Baseline Configuration Includes
# Include parent configuration files that this target extends
#################################################################################################################

include ./target/platform/linux_android_arm/config.mk


#################################################################################################################
# Application and Packaging — minSdk override for legacy hardware
#################################################################################################################

# Override the Play Store default from linux_android_arm.  Embedded devices in
# this family run as low as Android 7.1 (API 25) and the APK must install on
# whatever OS version the vendor image ships.  API 16 is the broadest gate;
# raise this per-platform if a specific board is known to run a newer OS.
EHS_ANDROID_MIN_SDK=16


################################### END OF CONFIGURATION ###################################################
