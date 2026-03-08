#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# HW independent linux build environment parameters 

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/config.mk

#target types are always the same for all linux so just use one file
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/

#These are the core mandatory target apecific porting components needed to run eRT. 

ifdef EHS_FILESYSTEM_SUPPORT
ifneq ($(EHS_FILESYSTEM_SUPPORT),stubbed)
    OBJECTS += target_file.$(OBJ)
endif
else
   # All GNU platforms will default have linux support unless it is set to none or stubbed 
   OBJECTS += target_file.$(OBJ)
   EHS_FILESYSTEM_SUPPORT=yes
endif
OBJECTS += target_process.$(OBJ) 
OBJECTS += target_main.$(OBJ)
OBJECTS += target_math.$(OBJ) 


# We nearly always need this for GNU targets
  LIB += z

  #LIB += ssl
  
ifdef EHS_MINGW2
	LIB+=archive-2
else
	ifdef EHS_SKIP_GNULIBRARIES
		LIB+=archive
	else			
		LIB+=archive	
	endif
endif

export EHS_DEBIAN_VERSION

ifdef EHS_ANDROID
	#doing libidn in url.mk to ensure oorder of statics is correct
	#LIB+=idn
else
    ifndef EHS_SKIP_GNULIBRARIES
    	ifeq ($(EHS_DEBIAN_VERSION),8)
           LIB+=idn
		else ifeq ($(EHS_DEBIAN_VERSION),9)
           LIB+=idn2
		else ifeq ($(EHS_DEBIAN_VERSION),10)
           LIB+=idn2
		else ifeq ($(EHS_DEBIAN_VERSION),11)
           LIB+=idn2
       	else   
	   	   ifneq ($(EHS_MINGW),)    
        		#LIB+=idn
			else
#default is now libidn2
			    LIB+=idn2
			endif
       	endif
    endif
endif
