#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

# Todo this is abit of an an outlier - should consider moving to the target directory as this is inked to target binaries.
# Note this is a bit of an exception that we are calling library from a common path. 
# This is because we use the XML uild system to build XML lib rather than link to it here (statically or dynamically)
# XML is the only library we consider core to EHS, though it currently isn't used for core EHS other than potentialy for devmanmonitor processing. 

include $(EHS_COMMON_HAL_PATH)/xml/deps.mk


VPATH+= $(EHS_COMMON_HAL_PATH)/xml #nothing in here really

ifdef EHS_INCLUDE_XML_SUPPORT
	ifneq ($(EHS_HOST_DEBIAN_BUILD),)
	INC_DIRS += /usr/include/libxml2
	else
	INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)/libxml2
	endif

	#This is for playManager
	ifdef EHS_ANDROID
	LIB+=:libxml2.a 
	else
		LIB += xml2 
		ifdef EHS_MINGW
		#This is to make sure linxml2 doesn't prefix the symbols with random tat
		DEFS += LIBXML_STATIC
		endif
	endif
endif

# This is the minimal xmls parser used for Devman core:
OBJECTS+= hal_xml_minimal.$(OBJ)

