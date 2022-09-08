#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#
#
# Makefile fragment to build the graphics/video-specific code for EHS.

# Called by ../../platform/<platform-type>
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_GRAPHICS_PATH - path to the current directory (set by platform makefile)


EHS_PERIPHERALS_GUI_KEYBOARD=yes
EHS_PERIPHERALS_RCU=yes

# include sourcecode from this dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/deps.mk
VPATH+=$(EHS_TARGET_GRAPHICS_PATH)
INC_DIRS+=$(EHS_TARGET_GRAPHICS_PATH)

# include sourcecode from components dir in build
include $(EHS_TARGET_GRAPHICS_PATH)/Components/deps.mk
VPATH+= $(EHS_TARGET_GRAPHICS_PATH)/Components
INC_DIRS += $(EHS_TARGET_GRAPHICS_PATH)/Components

#include the component support library paths -- use pkgconfig??
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/glib-2.0
INC_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)/glib-2.0/include # target specific data is in here
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gio-unix-2.0
ifeq ($(EHS_DEBIAN_10),yes)
INC_DIRS += /usr/include/gtk-2.0
INC_DIRS += /usr/lib/x86_64-linux-gnu/gtk-2.0/include/
INC_DIRS += /usr/include/glib-2.0
INC_DIRS += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
INC_DIRS += /usr/include/cairo/
INC_DIRS += /usr/include/pango-1.0/
INC_DIRS += /usr/include/gdk-pixbuf-2.0/
INC_DIRS += /usr/include/atk-1.0
INC_DIRS += /usr/include/harfbuzz
#todo make the above arch independent
else
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gtk-2.0
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gtk-3.0
INC_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)/gtk-2.0/include # target specific data is in here
endif
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/cairo
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/pango-1.0
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gdk-pixbuf-2.0
INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/atk-1.0

# No extra lib paths - all are in the target_libs dir

# PPP: Moved to Video Diectory OBJECTS += target_dtv.$(OBJ)
OBJECTS+=target_viewport.$(OBJ) 
OBJECTS+=targetgfx_init.$(OBJ)

# Make the dependency list: note this depends on how the component libraries are generated 
# Permutations on static dynamic linking .
ifeq ($(EHS_GNU_OS),mingw32msvc)
LIB+= gobject-2.0-0
LIB+= gdk-win32-2.0-0
LIB+= gtk-win32-2.0-0
LIB+= glib-2.0-0
LIB+= gthread-2.0-0
LIB+= cairo
LIB+= expat-1#dependency of gtk
#LIB+= cairo-gobject
#LIB+= gmodule-2.0
#LIB+= gio-2.0
#LIB+=atk-1.0
#LIB+=gdk_pixbuf-2.0
#LIB+=pangowin32-1.0
#LIB+=gdi32
#LIB+=pangocairo-1.0
#LIB+=pango-1.0
else 
# non mingw gtk dependencies go here
# Following added for arm, but may not be necessary for "standard gtk"?
LIB+= z
LIB+= gthread-2.0
LIB+= Xcomposite
LIB+= pixman-1
LIB+= pangocairo-1.0
LIB+= stdc++
LIB+= pangoft2-1.0
LIB+= Xfixes
LIB+= Xrender
LIB+= Xau
LIB+= fontconfig
LIB+= freetype
LIB+= Xext
LIB+= gmodule-2.0
LIB+= pango-1.0
LIB+= gio-2.0
LIB+= xcb
LIB+= xcb-shm
LIB+= xcb-render
LIB+= X11
LIB+= atk-1.0
LIB+= gdk_pixbuf-2.0
# arm done
LIB+= gobject-2.0
LIB+= glib-2.0
LIB+= cairo
LIB+= gdk-x11-2.0
LIB+= gtk-x11-2.0

ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
#LIB+= gdk-x11-2.0
#LIB+= gtk-x11-2.0
LIB+= png12
#LIB+= expat
LIB+= expatw
LIB+= ssl
LIB += Xcursor
LIB += Xi
LIB += Xrandr
LIB+= Xdmcp
LIB+= Xinerama
LIB+= pangoxft-1.0
LIB+= Xdamage
LIB+= Xft
LIB+= pcre
LIB += crypto
LIB += selinux
LIB += ffi
LIB += lzma
LIB += rt
LIB += gcc_s

else #normal case
LIB+= expat#gtk dependency
#LIB+= gdk-x11-2.0
#LIB+= gtk-x11-2.0

ifeq ($(SYSTEM_VARIANT),OMAP4_TI)
#LIB+= uid
#LIB+= pcreposix
LIB+= selinux
LIB+= SM 
LIB+= ICE
LIB+= Xdmcp
LIB+= Xinerama
LIB+= Xi
LIB+= Xrandr
LIB+= pangoxft-1.0
LIB+= Xdamage
LIB+= Xft
LIB+= Xt
LIB+= Xcursor
#LIB+= png
LIB+= png12
else
## TODO2022 This needs to be conditional on the OS version  not whether a host builld or not
	ifeq ($(EHS_DEBIAN_10),yes)
		EHS_PNG_LIB := png16
    else
	    ifeq ($(EHS_PNG_LIB),)
		    EHS_PNG_LIB := png14
	    endif
    endif
	LIB+= $(EHS_PNG_LIB)
endif
endif

endif



