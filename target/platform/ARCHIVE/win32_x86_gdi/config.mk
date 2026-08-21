#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# config.mk - Configuration properties of the current platform
# 
# Called by ../../../Makefile
#

# @author: inx limited



# MUST SET the following for any component config: 
#EHS_ARCH, EHS_OS/ Use the GNU format and order that is created by the libraries etc.
#CLIB_USEHOST
EHS_GNU_ARCH=i586## this will pick up the glibc2.9 library
EHS_GNU_OS=win32-msvc## make sure we have no spaces after !!
EHS_TARGET_LIB_VARIANT=# all libraries need to be built


# ehs is more generic and doesn't use special libc magic.
EHS_ARCH=x86
EHS_OS=win32

#TOOLCHAIN_NAME=

#
# uncomment this variable if the platform requires graphics/video support
#EHS_GUI_SUPPORT=yes
#
# Set this to match one of the graphics types in EHS/target/graphics
#EHS_GUI=none
EHS_GUI_SUPPORT=gdi
#IS_RGBA=yes - delete this it is not used ..

#
# uncomment this variable if the platform requires audio / video support
 EHS_AV_SUPPORT=vlc

#
# uncomment this variable if the platform requires media manager support (e.g. SMIL, DLNA).
#EHS_VIDEO_SUPPORT=yes
#EHS_VIDEO=none
 EHS_MEDIA_SUPPORT=all

#
# uncomment this variable if the platform requires NETWORKING e.g. devman plugins 

EHS_NETWORKING_SUPPORT=all
#@todo we need an entry here for the URL components etc.
EHS_COMPONENT_NETWORKING_SUPPORT=all

#
# uncomment this variable if the platform requires devman monitor support
EHS_DEVMAN_SUPPORT=all
EHS_DEVMAN_MON_SUPPORT=yes
#todo there should be a better conversion of 'all' into each devman required - maybe scrap EHS_DEVMAN_SUPPORT?

#
# uncomment this variable if the platform needs to support deprecated toolkit
EHS_TOOLKIT_DEPRECATED=yes
#@todo remove all TK_DEPRECATE symbols - no longer used


