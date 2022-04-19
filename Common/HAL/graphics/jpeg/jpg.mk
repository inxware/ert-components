#
# Makefile fragment to build the jpg graphics files for EHS.

# Called by ../kernel.mk
#

# @author: inx limited
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

include $(EHS_COMMON_HAL_PATH)/graphics/jpeg/deps.mk

VPATH+= $(EHS_COMMON_HAL_PATH)/graphics/jpeg

OBJECTS += jcapimin.$(OBJ)
OBJECTS += jcapistd.$(OBJ)
OBJECTS += jctrans.$(OBJ)
OBJECTS += jcparam.$(OBJ)
OBJECTS += jdatadst.$(OBJ)
OBJECTS += jcinit.$(OBJ)
OBJECTS += jcmaster.$(OBJ)
OBJECTS += jcmarker.$(OBJ)
OBJECTS += jcmainct.$(OBJ)
OBJECTS += jcprepct.$(OBJ)
OBJECTS += jccoefct.$(OBJ)
OBJECTS += jccolor.$(OBJ)
OBJECTS += jcsample.$(OBJ)
OBJECTS += jchuff.$(OBJ)
OBJECTS += jcphuff.$(OBJ)
OBJECTS += jcdctmgr.$(OBJ)
OBJECTS += jfdctfst.$(OBJ)
OBJECTS += jfdctflt.$(OBJ)
OBJECTS += jfdctint.$(OBJ)
OBJECTS += jdapimin.$(OBJ)
OBJECTS += jdapistd.$(OBJ)
OBJECTS += jdtrans.$(OBJ)
OBJECTS += jdatasrc.$(OBJ)
OBJECTS += jdmaster.$(OBJ)
OBJECTS += jdinput.$(OBJ)
OBJECTS += jdmarker.$(OBJ)
OBJECTS += jdhuff.$(OBJ)
OBJECTS += jdphuff.$(OBJ)
OBJECTS += jdmainct.$(OBJ)
OBJECTS += jdcoefct.$(OBJ)
OBJECTS += jdpostct.$(OBJ)
OBJECTS += jddctmgr.$(OBJ)
OBJECTS += jidctfst.$(OBJ)
OBJECTS += jidctflt.$(OBJ)
OBJECTS += jidctint.$(OBJ)
OBJECTS += jidctred.$(OBJ)
OBJECTS += jdsample.$(OBJ)
OBJECTS += jdcolor.$(OBJ)
OBJECTS += jquant1.$(OBJ)
OBJECTS += jquant2.$(OBJ)
OBJECTS += jdmerge.$(OBJ)
OBJECTS += jcomapi.$(OBJ)
OBJECTS += jutils.$(OBJ)
OBJECTS += jerror.$(OBJ)
OBJECTS += jmemmgr.$(OBJ)
OBJECTS += jmemnobs.$(OBJ)
