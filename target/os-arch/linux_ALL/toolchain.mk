#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the MPL2.0 license. You should have received a copy of the 
# MPL2.0 (Mozilla Public License2.0) license with this file. If 
# not, please visit 
#	<https://www.mozilla.org/en-US/MPL/2.0/>
#---------------------------------------------------------------#

# This file can be included bny any linux hardware toolchain.mk file


#This is generally needed
ifndef EHS_ANDROID
LIB+=pthread
else
#This is included in bionic libc
#LIB+=pthread
endif
