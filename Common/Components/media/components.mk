#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#
# Makefile fragment to build the playerManager component code for EHS.

# Called by ../components.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

#Dependencies
#$(info Setting XMLS to yes)
EHS_INCLUDE_XML_SUPPORT=yes

include $(EHS_COMMON_COMPONENTS_PATH)/media/deps.mk

OBJECTS += playManager.$(OBJ)
OBJECTS += media_components.$(OBJ)


VPATH+= $(EHS_COMMON_COMPONENTS_PATH)/media

