#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# @file config.mk 
# inxware ERT configuration file for linux_armv7l_clang_gtk
# @author: inx limited

#Target Platform Uses
# This is the horrific arm / raspberry pi config with scraped components and uses a clang compiler 
# so that is can potentially integrate green grass components built with the same compiler
# It should be refactored to used docker and Debians arm  cross compile environment for Rasberrian
# All contributed middlware is provided by a munge of things we want rid of ???

include ./target/platform/linux_armv7l_clang_debian9/config.mk


################################################################################################################
# Configure debug/production levels
################################################################################################################
#DEBUG OPTIONS
EHS_DEBUGALL=true
#endif

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################

#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http 

# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
#This include RCUs, text displays, etc.
EHS_PERIPHERAL_DEVICE_SUPPORT=all
# uncomment this variable if the platform requires graphics/video support
# Set this to match one of the graphics types in EHS/target/graphics
# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
EHS_AV_SUPPORT=devmanonly
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
EHS_MEDIA_SUPPORT=all

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
EHS_TOOLKIT_DEPRECATED=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################


#EHS_PNG_LIB := png16



