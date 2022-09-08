#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# HW independent linux build environment parameters 
#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/platform
KERNEL_HEADERS_RELPATH =$(KERNEL_VERSION)/headers/include/

VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/

ifndef EHS_DEBIAN_VERSION
## the following iline s needed for old make file s
   export EHS_DEBIAN_10=yes
else
  ifeq (EHS_DEBIAN_VERSION , 10)
      export EHS_DEBIAN_10=yes
  endif
endif

# todo we might want a common linux bin to be created
# moves to specific linux: OBJECTS += targetos_init.$(OBJ) 

OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)

LIB+=:libehs.a
#dependency of CURL???
ifdef EHS_ANDROID
	#doing libidn in url.mk to ensure oorder of statics is correct
	#LIB+=idn
else
    ifndef EHS_SKIP_GNULIBRARIES
       ifeq ($(EHS_DEBIAN_10),yes)
           LIB+=idn2
       else       
           LIB+=idn
       endif
    endif
endif

#this should be in file, but having problems with conditionals
ifdef EHS_ANDROID
   LIB +=:libarchive.a
else
  ifndef EHS_SKIP_GNULIBRARIES
     LIB +=archive
  endif
endif



