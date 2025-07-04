#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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

# Some additional features related to the GUI/graphics subsustem
EHS_PERIPHERALS_GUI_KEYBOARD=yes
EHS_PERIPHERALS_RCU=yes

# We get the GTK library headers and binaries from different places depending on the platform...

ifneq ($(EHS_HOST_DEBIAN_BUILD),)
  ifeq ($(EHS_DEBIAN_VERSION),11)
  	EHS_PNG_LIB_DEFAULT = png16
  else ifeq ($(EHS_DEBIAN_VERSION),10)
	EHS_PNG_LIB_DEFAULT = png16
  else ifeq ($(EHS_DEBIAN_VERSION),9)
	EHS_PNG_LIB_DEFAULT = png16
  else
  #Still use GTK-2 or do we still need to build for GTK-1 for 32 bit debian targets?
	EHS_PNG_LIB_DEFAULT = png14
  endif
  	ifeq ($(EHS_ARCH),arm64)
		#This is for the rest (seems we otherwise default to gtk3)
		INC_DIRS += /usr/include/gtk-3.0
		INC_DIRS += /usr/lib/x86_64-linux-gnu/gtk-2.0/include/
		INC_DIRS += /usr/include/glib-2.0
		INC_DIRS += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
		LIB+= gdk-3
		LIB+= gtk-3
		LIB+= gdk_pixbuf-2.0
	else ifeq ($(EHS_ARCH),arm)
	  # This is for armv7 debian 10..
		INC_DIRS += /usr/include/gtk-3.0
		INC_DIRS += /usr/include/glib-2.0/
		INC_DIRS += /usr/lib/arm-linux-gnueabihf/glib-2.0/include/
		LIB+= gdk-3
		LIB+= gtk-3
		LIB+= gdk_pixbuf-2.0
		EHS_PNG_LIB_DEFAULT = png16
	else
		#This is for the rest (usually x86_64)
		INC_DIRS += /usr/include/gtk-2.0
		INC_DIRS += /usr/lib/x86_64-linux-gnu/gtk-2.0/include/
		INC_DIRS += /usr/include/glib-2.0
		INC_DIRS += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
		LIB+= gdk-x11-2.0
		LIB+= gtk-x11-2.0 
		LIB+= gdk_pixbuf-2.0
	endif
	INC_DIRS += /usr/include/cairo/
	INC_DIRS += /usr/include/pango-1.0/
	INC_DIRS += /usr/include/gdk-pixbuf-2.0/
	INC_DIRS += /usr/include/atk-1.0
	INC_DIRS += /usr/include/harfbuzz

  # Debian Host system libraries 
  #	LIB+= z This is done elswhere
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
	# arm done
	LIB+= gobject-2.0
	LIB+= glib-2.0
	LIB+= cairo

else
# For Debian host builds
# For targets where we get the dependencies from the ert-contrib-middleware repo:

  ifeq ($(EHS_GNU_OS),mingw32posix)
  # Mingw is using  GTK3
    INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gtk-3.0 
	LIB += gobject-2.0
	LIB += gtk-3
	LIB += gdk-3
	LIB += glib-2.0
	LIB += gthread-2.0
	LIB += cairo
	LIB += expat#dependency of gtk
	LIB += gdi32
   else 
   # Linux targets are still using GTK2 (e.g. 32 bit debian 8)
   	EHS_PNG_LIB_DEFAULT = png

     INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gtk-2.0 
     INC_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)/gtk-2.0/include # target specific data is in here
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
	endif
# ert-contrib-middleware Dependencies common for all GTK versions (non-gost built targets).
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/glib-2.0
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)/glib-2.0/include
# target specific data is in here
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gio-unix-2.0
# Graphical dependencies of GTK that seem tobe common across GTK1-3
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/harfbuzz
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/cairo
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/pango-1.0
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gdk-pixbuf-2.0
	INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/atk-1.0
endif

# Stranger builder options (Addtive to above):

# Following added for arm RPI Raspbian build, but may not be necessary for "standard gtk"?
ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
	EHS_PNG_LIB_DEFAULT=png12
	LIB+= expatw
#todo - we might need to exclude LIB+=expat in the above.
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
#todo2022 - why is these here as a GFX dependency?
	LIB += crypto
	LIB += selinux
	LIB += ffi
	LIB += lzma
	LIB += rt
	LIB += gcc_s
else 
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
		EHS_PNG_LIB_DEFAULT=png12
	else
        ##############################################################################################################################
		# REMAINDER OF NORMAL CONFIG GOES HERE:
		#############################################################################################################################

		##############################################################################################################################
	endif# end not OMAP4_TI
endif# end not RASPBERRYPI

#Some dependencies vary randomly and might be overriden in platform config.mk files. 
#e.g. PNG lib so this should be set via a variable in one place: 
ifeq ($(EHS_PNG_LIB),)
#currently using 16 as the default but we may move to 16 and override in platforms that 14 stll
   ifeq ($(EHS_PNG_LIB_DEFAULT),)
   	  LIB += png16
   else 
      LIB += $(EHS_PNG_LIB_DEFAULT)
	endif
else
	LIB += $(EHS_PNG_LIB)
endif


# THINGS WE DO FOR ALL PLATFORMS:

# ert-component objects that need building and linking:
OBJECTS+=target_viewport.$(OBJ) 
OBJECTS+=targetgfx_init.$(OBJ)
