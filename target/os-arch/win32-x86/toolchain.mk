#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#


# WARNING: THIS FILE IS NOT CURRENTLY ACTIVE AND IT'S FORMAT IS DEPRICATED
#
#
# Target-specific definitions for the EHS Microsoft Windows/Visual Studio target.
# This file is called from a makefile
#
#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#

# Lua includes directory


# Lua library "todo this should go to the LUA make file
LUA_LIB= "..\environment\lua\target_binaries\win_x86\static_lib\lua5.1.lib"

#
# Toolset specific directories
VSINSTALLDIR=/cygdrive/c/Program\ Files/Microsoft\ Visual\ Studio\ 8/
VCINSTALLDIR=$(VSINSTALLDIR)/vc
DevEnvDir=$(VSINSTALLDIR)\Common7\IDE
FrameworkDir=/cygdrive/c/Windows/Microsoft.NET/Framework

# DYNAMICLINK=

PATH+=:/cygdrive/c/Program\ Files/Microsoft\ Visual\ Studio\ 8/Common7/IDE
DEV_PATH=/cygdrive/c/Program\ Files/Microsoft\ Visual\ Studio\ 8/vc/bin
CC=@$(DEV_PATH)/cl.exe
CPP=@$(DEV_PATH)/cl.exe
RC=$(DEV_PATH)/rc.exe
LINK=@$(DEV_PATH)/link.exe
INC=$(foreach i,$(INC_DIRS),/I$i)
INC+=/I"C:\Program Files\Microsoft Visual Studio 8\vc\include"
INC+=/I"C:\Program Files\Microsoft Visual Studio 8\vc\PlatformSDK\include"
INC+=/I"C:\Program Files\Microsoft Visual Studio 8\VC\atlmfc\include"
#INC+=/I"vlc"
#INC+=/I"..\environment\lua\lua-5.1.3\src"
#hide if building for release

# Compiler flags
# /Od - disable optimization
# /D - defines constants/macros
# /Gm  - Enables minimal rebuild
# /EHsc - Exception handling model.sc - enable C++ exceptions 
# /RTC1 - Enables run-time error checking. 1 = Stack frame checking & used variable without initialization
# /MDd - Use multithreaded debug DLL
# /GS- - disable buffer security check
# /Fd"Debug\vc80.pdb" /c /TP  /w /Z7
# /Fo"Debug\\" - where object files are created [not used]
# /Fd"Debug\vc80.pdb" - renames program database file (.pdb)
# /W3  - Warning level
# /nologo - suppress display of sign-on banner
# /c  - compile only
# /Wp64 - detect 64 bit portability problems
# /Zi - generates complete debugging information
# /TP - Compile as C++ code 


CFLAGS=$(foreach i,$(DEFS),/D"$i")
CPPFLAGS=$(foreach i,$(DEFS),/D"$i")

# ToDo: Have a switch for debug versions
# Statically linked version
ifdef DYNAMICLINK
CFLAGS+=/nologo /Od $(INC) /D"WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /MDd /W3 /c /Wp64 /Zi
CPPFLAGS+=/Od $(INC) /D"WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /MDd /W3 /nologo /c /Wp64 /Zi
else
CFLAGS+=/nologo /Od $(INC) /D"WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /MTd /W3 /c /Wp64 /Zi
#CFLAGS+=/Od $(INC) /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /Gm /EHsc /RTC1 /MTd /W3 /nologo /c /ZI
CPPFLAGS+=/Od $(INC) /D"WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /MTd /W3 /nologo /c /Wp64 /Zi
endif

# deprecated flags?? /D "_LIB" /D "_CRT_NONSTDC_NO_DEPRECATE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /EHsc /RTC1 /MDd /GS- /Fd"Debug\vc80.pdb" /c /TP  /w /Z7
LIB=/LIBPATH:"C:\Program Files\Microsoft Visual Studio 8\vc\LIB"
LIB+=/LIBPATH:"C:\Program Files\Microsoft Visual Studio 8\vc\ATLMFC\LIB"
LIB+=/LIBPATH:"C:\Program Files\Microsoft Visual Studio 8\vc\PlatformSDK\lib"
LIB+=/LIBPATH:"C:\Windows\Microsoft.NET\Framework\v2.0.50727\lib"

LNKFLAGS=/OUT:"$(TARGET_NAME).$(EXE)" /INCREMENTAL /NOLOGO /SUBSYSTEM:WINDOWS /DEBUG $(LUA_LIB) $(LIB)
#LNKFLAGS=/OUT:"$(TARGET_NAME).$(EXE)" /INCREMENTAL /NOLOGO /SUBSYSTEM:WINDOWS /DEBUG $(LIB)
LNKFLAGS+=/MAP:"$(TARGET_NAME).map"
LNKFLAGS+=user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib 
LNKFLAGS+=comctl32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib


#LNKFLAGS+=-o$(TARGET_NAME)
#LNKFLAGS+=-mwindows
# provide sockets support
#PPP:LIB+=ws2_32


todo the following $(INC) and $(LNKFLAGS) for the vpath part needs to be changed to swap the slashes from / to \ and possibly remove some cygdrive/ junk too.
LNKFLAGS+=$(foreach i,$(LIB),-l$i)
LNKFLAGS+=$(foreach i,$(LIB_DIRS),-L$i)
#RCFLAGS=$(INC) /d "_UNICODE" /d "UNICODE"


RCFLAGS=$(INC) /d "_UNICODE" /d "UNICODE"

%.res : %.rc
	$(RC) $(RCFLAGS) /fo "$(EHS_BUILD_PATH)/$@" $<
#
# File extensions
#
EXE = exe
OBJ = obj
FINAL=$(EXE)

CLEAN_FILES=*.manifest *.exp *.lib *.ilk *.pdb *.idb
