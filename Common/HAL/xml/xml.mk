#
# Makefile fragment to build the HAL file support for EHS.

# Called by ../kernel.mk
#

# @author: Patrick Beaumont
# @version: $Revision: 43 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
#
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
#VPATH+= $(ENVIRONMENT_PATH)/libxml2/src/include # libxml2 headersa
INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)/libxml2
# library is in standard position.

OBJECTS+= hal_xml_minimal.$(OBJ)
# Reference target XML support library.
# todo - should we include libxml2 here or should it be in a separate make file
#@todo this should move to the base full switches.

