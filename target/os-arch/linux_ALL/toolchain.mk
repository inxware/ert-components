#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# This file can be included bny any linux hardware toolchain.mk file


#This is generally needed
ifndef EHS_ANDROID
LIB+=pthread
else
#This is included in bionic libc
#LIB+=pthread
endif
