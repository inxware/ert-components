#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
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

# @Todo this is abit of an an outlier - should consider moving to the target directory as this is inked to target binaries.


include $(EHS_COMMON_HAL_PATH)/url/deps.mk

# Tell the code we are in business 
DEFS+=EHS_NETWORKING_SUPPORT 

# need this for win32 & mingw builds - stops trying to link to __imp_ prefixed functions pointing at the dyanamic dll version.

CFLAGS+= -DCURL_STATICLIB 

VPATH+= $(EHS_COMMON_HAL_PATH)/url #nothing in here really
#INC_DIRS+=$(EHS_COMPONENT_SUPPORT_INCLUDE)/curl

#DEFS+= CURL_STATICLIB# We will statically link this (Better this way and we don't have dlls for windows)
OBJECTS+= hal_url.$(OBJ)
# Reference target XML support library.



ifdef EHS_MINGW
CFLAGS += -DUSE_WINDOWS_SSPI
CFLAGS += -DUSE_SSLEAY -DHAVE_OPENSSL_ENGINE_H
CFLAGS += -DUSE_LIBSSH2 -DHAVE_LIBSSH2_H
LIB += ssh2
LIB += curldll
LIB += rtmp
LIB += z
LIB += ssh2dll
LIB += idn.dll
#LIB += zdll
LIB += crypto
else
ifdef EHS_ANDROID
LIB +=:libcurl.a
LIB +=:libssl.a
LIB +=:libcrypto.a
LIB +=:libidn.a

#LIB +=curl
#LIB+=crypto
#LIB+=ssl

else
LIB += curl
endif
endif


#todo move this to target bit?


#@todo this should move to the base full switches.

