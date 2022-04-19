#
# Makefile for EHS
#
#

# @author: inx limited
# @version: $Revision: 1264 $
# @date: $Date: 2006-10-30 05:05:44 +0000 (Mon, 30 Oct 2006) $
# 
# Copyright (c) inx limited, 2007. All rights reserved.
#
# Overrides from command line:
# TARGET - specify a different target
# REVOPT=-V - if a build with the revision number is required
# TK_DEPRECATE=no if the deprecated toolkit is not required in this build
#
# Targets:
#  clean
#  depend - recreate file dependencies
REVOPT=-V
TARGET=win_x86
#TARGET=tap_topfield5800
#TARGET=linux_sh4
TK_DEPRECATE = yes

TARGET_NAME=ehs_$(TARGET)

################################################################################
# Standard directory paths
#
SCRIPT_PATH=../../../scripts
ENVIRONMENT_PATH=../../../environment
EHS_ROOT_PATH=../..
EHS_BUILD_PATH=.
## Obsolete EHS_TARGET_PATH=$(EHS_ROOT_PATH)/target/$(TARGET)
EHS_HAL_HEADERS_PATH=$(EHS_COMMON_PATH)/HAL/include
EHS_COMMON_PATH=$(EHS_ROOT_PATH)/Common
EHS_COMMON_KERNEL_PATH=$(EHS_COMMON_PATH)/Kernel
EHS_COMMON_HAL_PATH=$(EHS_COMMON_PATH)/HAL
EHS_COMMON_COMPONENTS_PATH=$(EHS_COMMON_PATH)/Components
EHS_COMMON_TOOLKIT_CORE_PATH=$(EHS_COMMON_COMPONENTS_PATH)/core
EHS_COMMON_TOOLKIT_DTV_PATH=$(EHS_COMMON_COMPONENTS_PATH)/dtv
EHS_COMMON_TOOLKIT_GUI_PATH=$(EHS_COMMON_COMPONENTS_PATH)/gui
EHS_COMMON_TOOLKIT_PNG_PATH=$(EHS_COMMON_COMPONENTS_PATH)/gui/png

################################################################################
# Standard include paths
#
INC_DIRS=$(EHS_COMMON_KERNEL_PATH) $(EHS_COMMON_COMPONENTS_PATH) $(EHS_TARGET_PATH) 
INC_DIRS+=$(EHS_HAL_HEADERS_PATH)
INC_DIRS+=$(EHS_COMMON_TOOLKIT_CORE_PATH) $(EHS_COMMON_TOOLKIT_DTV_PATH) $(EHS_COMMON_TOOLKIT_GUI_PATH)
INC_DIRS+=$(EHS_COMMON_HAL_PATH)/graphics 
INC_DIRS += $(EHS_COMMON_HAL_PATH)/tcp
INC_DIRS+=$(EHS_COMMON_TOOLKIT_PNG_PATH) $(EHS_COMMON_HAL_TCP_PATH)
INC_DIRS+=.

# standard definitions for Csource
DEFS=


################################################################################
# Target-specific tool definitions. 
#  Uses $(INC_DIRS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#
include $(EHS_TARGET_PATH)/toolchain.mk


#################################################################################
# First rule: defines what to build
#
all: $(TARGET_NAME).$(FINAL)

#
# Where to look for source code
#
VPATH=.
VPATH+=: $(EHS_TARGET_PATH)
VPATH+=: $(EHS_HAL_HEADERS_PATH)
VPATH+=: $(EHS_COMMON_PATH)
VPATH+=: $(EHS_COMMON_KERNEL_PATH)
VPATH+=: $(EHS_COMMON_HAL_PATH)/file
VPATH+=: $(EHS_COMMON_HAL_PATH)
VPATH+=: $(EHS_COMMON_HAL_PATH)/graphics
VPATH+=: $(EHS_COMMON_HAL_PATH)/logger
VPATH+=: $(EHS_COMMON_HAL_PATH)/memory
VPATH+=: $(EHS_COMMON_HAL_PATH)/string
VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)
VPATH+=: $(EHS_COMMON_TOOLKIT_CORE_PATH)
VPATH+=: $(EHS_COMMON_TOOLKIT_DTV_PATH)
VPATH+=: $(EHS_COMMON_TOOLKIT_GUI_PATH)
VPATH+=: $(EHS_COMMON_TOOLKIT_PNG_PATH)

#
# add required object files to OBJECTS
#
OBJECTS+=tgttest_main.$(OBJ)
OBJECTS+=tgttest_console.$(OBJ)
OBJECTS+=tgttest_file.$(OBJ)
OBJECTS+=tgttest_graphics.$(OBJ)
OBJECTS+=tgttest_mem.$(OBJ)
OBJECTS+=tgttest_time.$(OBJ)
OBJECTS+=console_queue.$(OBJ)
OBJECTS+=tgttest_string.$(OBJ)
OBJECTS+=tgttest_viewport.$(OBJ)
OBJECTS+=tgttest_widget.$(OBJ)
OBJECTS+=tgttest_dummy.$(OBJ)
OBJECTS+=hal.$(OBJ)
OBJECTS+=hal_mem.$(OBJ)
OBJECTS+=graphics.$(OBJ)
OBJECTS+=html.$(OBJ)
OBJECTS+=font.$(OBJ)
OBJECTS+=utils.$(OBJ)
OBJECTS+=widget.$(OBJ)
OBJECTS += widget_textbox.$(OBJ)
OBJECTS += hal_string.$(OBJ)
OBJECTS += hal_file.$(OBJ)
OBJECTS += hal_logger.$(OBJ)

include $(EHS_HAL_HEADERS_PATH)/deps.mk
include $(EHS_TARGET_PATH)/target-test.mk

# Can't use predefined implicit rules here - we might need to generate .obj, rather than .o
# therefore create custom implicit rules.
%.$(OBJ): %.c
	@echo $<
	@$(CC) $(CFLAGS) $< 

%.$(OBJ): %.cpp
	$(CPP) $(CPPFLAGS) $< 

$(TARGET_NAME).$(EXE) : $(OBJECTS)
	$(LINK) $(OBJECTS) $(LNKFLAGS)

# rebuild all dependencies
depend:
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/Kernel
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/HAL/graphics
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/HAL/graphics/png
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/HAL/include
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/HAL/logger
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/Components/core
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/Components/dtv
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/Components/gui
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/Common/Components/deprecated
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/linux_sh4
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/linux_sh4/Components/dtv
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/tap_topfield5800
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/tap_topfield5800/Components/dtv
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/win_x86
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/win_x86/Components
	$(EHS_ROOT_PATH)/makedep.sh $(EHS_ROOT_PATH)/target/win_x86/Components/dtv

clean:
	rm $(TARGET_NAME).$(EXE) $(OBJECTS) $(CLEAN_FILES)
