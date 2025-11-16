#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#
# Makefile fragment to build the png graphics files for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
# Predefined variables

#  OBJ - File extension for object files

#  NOREV - Defined if no revision number checking is to be performed
# Expected variables

#  OBJECTS - list of object files added by this makefile
#  VPATH - where to look for source code

include $(EHS_COMMON_HAL_PATH)/graphics/png/deps.mk

OBJECTS+= adler32.$(OBJ) compress.$(OBJ) crc32.$(OBJ) deflate.$(OBJ) 
# gzio.$(OBJ) 
OBJECTS+= infback.$(OBJ) inffast.$(OBJ) inflate.$(OBJ) inftrees.$(OBJ) png.$(OBJ) pngerror.$(OBJ) pngget.$(OBJ) pngmem.$(OBJ) pngpread.$(OBJ) pngread.$(OBJ) pngrio.$(OBJ) pngrtran.$(OBJ) pngrutil.$(OBJ) pngset.$(OBJ) pngtrans.$(OBJ) pngvcrd.$(OBJ) pngwio.$(OBJ) 
#pngwrite.$(OBJ) pngwtran.$(OBJ) pngwutil.$(OBJ) 
OBJECTS += trees.$(OBJ) uncompr.$(OBJ) zutil.$(OBJ)

VPATH+= $(EHS_COMMON_HAL_PATH)/graphics/png
