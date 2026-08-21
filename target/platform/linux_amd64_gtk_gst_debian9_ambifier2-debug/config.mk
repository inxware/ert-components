#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_x86_gtk_gst_ambifier2-debug
# @author: inx limited

#debug version is copied from the non-debug release version.
include ./target/platform/linux_amd64_gtk_gst_debian9_ambifier2/config.mk

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################

# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
# SYSTEM_VARIANT is primarilly for conditional compilation for very specific features 
# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).  

#Note we (probably) have this extra specific system variant so the haacks file add the ambifier2 Unity app to the package.
SYSTEM_VARIANT=ambifier2-debug
#Need to build ehs-debug version to not lash with non ambifier2 version (e.g. for tsa.ambifier.com migration)
DEBIAN_PACKAGE_NAME=ehs-debug
INXWARE_TARGETENV_HACKS=ambifier2

################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
#DEBUG OPTIONS
EHS_DEBUGALL=true

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                         #

EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
################################### END OF TOOLBOX CONFIGURATION ###################################################
