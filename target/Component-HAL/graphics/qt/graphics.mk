# Copyright (C) 2025 inx limited, UK - All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit:
#  <https://www.gnu.org/licenses/lgpl-3.0.txt>
#
#---------------------------------------------------------------
# Qt Graphics HAL Build Configuration
#---------------------------------------------------------------

# Some additional features related to the GUI/graphics subsustem
#EHS_PERIPHERALS_GUI_KEYBOARD=yes
#EHS_PERIPHERALS_RCU=yes

# We get the GTK library headers and binaries from different places depending on the platform...
ifneq ($(EHS_HOST_DEBIAN_BUILD),)
	ifneq ($(filter 9 10 11 12 13,$(EHS_DEBIAN_VERSION)),)
		EHS_PNG_LIB_DEFAULT = png16
	else
		# Use GTK-2 or do we still need to build for GTK-1 for 32 bit debian targets?
		EHS_PNG_LIB_DEFAULT = png14
	endif

	
#	INC_DIRS += /usr/include/cairo/
#	INC_DIRS += /usr/include/pango-1.0/
#	INC_DIRS += /usr/include/gdk-pixbuf-2.0/
#	INC_DIRS += /usr/include/atk-1.0
#	INC_DIRS += /usr/include/harfbuzz

	# Debian Host system libraries
	#   LIB += z This is done elswhere

	#LIB += gthread-2.0
	#LIB += Xcomposite
	#LIB += pixman-1
	#LIB += pangocairo-1.0
	LIB += stdc++
	#LIB += pangoft2-1.0
	#LIB += Xfixes
	#LIB += Xrender
	#LIB += Xau
	LIB += fontconfig
	LIB += freetype
	#LIB += Xext
	#LIB += gmodule-2.0
	#LIB += pango-1.0
	LIB += gio-2.0
	#LIB += xcb
	#LIB += xcb-shm
	#LIB += xcb-render
	#LIB += X11
	#LIB += atk-1.0
	# arm done

	#LIB += gobject-2.0
	#LIB += glib-2.0
	#LIB += cairo

	ifdef EHS_GUI_SUPPORT_QT6
# Qt6 libraries
		DEFS += EHS_GUI_SUPPORT_QT6
		LIB += Qt6Core
		LIB += Qt6Gui
		LIB += Qt6Qml
		LIB += Qt6Quick

		ifeq ($(EHS_HOST_DEBIAN_BUILD),x86_64)
			LIB_DIRS += /usr/lib/x86_64-linux-gnu/qt6
		else ifeq ($(EHS_HOST_DEBIAN_BUILD),x86)
			LIB_DIRS += /usr/lib/i386-linux-gnu
		endif


		# Qt6 include paths (Debian 11/12 layout)
		ifeq ($(EHS_ARCH),arm64)
			QT_INCLUDE_BASE = /usr/include/aarch64-linux-gnu/qt6
		else ifeq ($(EHS_GNU_ARCH),x86_64)
			QT_INCLUDE_BASE = /usr/include/x86_64-linux-gnu/qt6
		else ifeq ($(EHS_HOST_DEBIAN_BUILD),x86)
			QT_INCLUDE_BASE = /usr/include/i386-linux-gnu/qt6
		else
			QT_INCLUDE_BASE = /usr/include/qt6
		endif
	else 
# Qt5 libraries
		LIB += Qt5Core
		LIB += Qt5Gui
		LIB += Qt5Qml
		LIB += Qt5Quick

		# Qt5 library paths
		ifeq ($(EHS_HOST_DEBIAN_BUILD),x86_64)
			LIB_DIRS += /usr/lib/x86_64-linux-gnu/qt5
		else ifeq ($(EHS_HOST_DEBIAN_BUILD),x86)
			LIB_DIRS += /usr/lib/i386-linux-gnu
		endif

		# Qt5 include paths (Debian 11/12 layout)
		ifeq ($(EHS_ARCH),arm64)
			QT_INCLUDE_BASE = /usr/include/aarch64-linux-gnu/qt5
		else ifeq ($(EHS_GNU_ARCH),x86_64)
			QT_INCLUDE_BASE = /usr/include/x86_64-linux-gnu/qt5
		else ifeq ($(EHS_HOST_DEBIAN_BUILD),x86)
			QT_INCLUDE_BASE = /usr/include/i386-linux-gnu/qt5
		else
			QT_INCLUDE_BASE = /usr/include/qt5
		endif
	endif

	# Qt headers require C++17 so must only be on the C++ include path
	CXX_INC_DIRS += $(QT_INCLUDE_BASE)
#	CXX_INC_DIRS += $(QT_INCLUDE_BASE)/QtCore
	CXX_INC_DIRS += $(QT_INCLUDE_BASE)/QtGui
#	CXX_INC_DIRS += $(QT_INCLUDE_BASE)/QtQml
	CXX_INC_DIRS += $(QT_INCLUDE_BASE)/QtQuick
# todo this may eed to go into the toolchain.mk?
	CPPFLAGS += -std=c++17


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
		LIB += expat  #dependency of gtk
		LIB += gdi32
   else
		# Linux targets are still using GTK2 (e.g. 32 bit debian 8)
		EHS_PNG_LIB_DEFAULT = png

		INC_DIRS += $(EHS_COMPONENT_SUPPORT_INCLUDE)/gtk-2.0
		INC_DIRS += $(EHS_COMPONENT_SUPPORT_LIBS)/gtk-2.0/include # target specific data is in here
		LIB += z
		LIB += gthread-2.0
		LIB += Xcomposite
		LIB += pixman-1
		LIB += pangocairo-1.0
		LIB += stdc++
		LIB += pangoft2-1.0
		LIB += Xfixes
		LIB += Xrender
		LIB += Xau
		LIB += fontconfig
		LIB += freetype
		LIB += Xext
		LIB += gmodule-2.0
		LIB += pango-1.0
		LIB += gio-2.0
		LIB += xcb
		LIB += xcb-shm
		LIB += xcb-render
		LIB += X11
		LIB += atk-1.0
		LIB += gdk_pixbuf-2.0
		# arm done

		LIB += gobject-2.0
		LIB += glib-2.0
		LIB += cairo
		LIB += gdk-x11-2.0
		LIB += gtk-x11-2.0
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

# Stranger builder options (addtive to above):

# Following added for arm RPI Raspbian build, but may not be necessary for "standard gtk"?
ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
	EHS_PNG_LIB_DEFAULT=png12
	LIB += expatw
	#todo - we might need to exclude LIB +=expat in the above.

#	LIB += ssl
	LIB += Xcursor
	LIB += Xi
	LIB += Xrandr
	LIB += Xdmcp
	LIB += Xinerama
	LIB += pangoxft-1.0
	LIB += Xdamage
	LIB += Xft
	LIB += pcre
	#todo2022 - why is these here as a GFX dependency?
	LIB += selinux
	LIB += ffi
	LIB += lzma
	LIB += rt
	LIB += gcc_s

else

	ifeq ($(SYSTEM_VARIANT),OMAP4_TI)
		#LIB += uid
		#LIB += pcreposix
		LIB += selinux
		LIB += SM
		LIB += ICE
		LIB += Xdmcp
		LIB += Xinerama
		LIB += Xi
		LIB += Xrandr
		LIB += pangoxft-1.0
		LIB += Xdamage
		LIB += Xft
		LIB += Xt
		LIB += Xcursor
		EHS_PNG_LIB_DEFAULT=png12
	else
		##############################################################################################################################
		# REMAINDER OF NORMAL CONFIG GOES HERE:
		#############################################################################################################################

		##############################################################################################################################
	endif # end not OMAP4_TI
endif # end not RASPBERRYPI

# Some dependencies vary randomly and might be overriden in platform config.mk files.
# e.g. PNG lib so this should be set via a variable in one place:
ifeq ($(EHS_PNG_LIB),)
	# Currently using 16 as the default but we may move to 16 and override in platforms that still use 14
	ifeq ($(EHS_PNG_LIB_DEFAULT),)
		LIB += png16
	else
		LIB += $(EHS_PNG_LIB_DEFAULT)
	endif
else
	LIB += $(EHS_PNG_LIB)
endif

# THINGS WE DO FOR ALL PLATFORMS:

# Qt5 requires position-independent code
CPPFLAGS += -fPIC

# Disable PIE for linking (kernel library may not be compiled with -fPIE)
LD_SWITCHES += -no-pie

# ert-component objects that need building and linking:
OBJECTS += target_viewport.$(OBJ)
OBJECTS += targetgfx_init.$(OBJ)
OBJECTS += ertqt.$(OBJ)
OBJECTS += qt_main_integration.$(OBJ)
