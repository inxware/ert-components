#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# Makefile fragment to build target specific component and support code.
# Called by ../../platform/<platform-type>

#  OBJ - File extension for object files

# This flag is used for Zephyr-specific build conditionals
EHS_ZEPHYR=yes

# Expected variables
#  OBJECTS - list of object files added by this makefile
#  INC_DIRS  - target-specific include paths
#  VPATH - where to look for source code
#  EHS_TARGET_OS_HW_PATH - path to the current directory (set by platform makefile)

# Default OS Features Supported
ifneq ($(EHS_FILESYSTEM_SUPPORT),none)
ifndef EHS_FILESYSTEM_SUPPORT
	EHS_FILESYSTEM_SUPPORT=posix
endif
endif

# Target path
_TARGET_PATH = $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/

# Include source code from this dir in build
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/zephyr-arm/

# Zephyr-specific defines
DEFS += ZEPHYR_RTOS=1
DEFS += EHS_ZEPHYR_SUPPORT=1
EHS_ZEPHYR_SUPPORT=1

# Add Zephyr build include paths if available
ifdef ZEPHYR_BUILD_DIR
INC_DIRS += $(ZEPHYR_BUILD_DIR)/zephyr/include/generated
endif

# Include version string
-include TARGET.cfg
ifdef TARGET
DEFS += 'TARGET_OS_VERSION_STRING="$(shell head -c -1 ./Releases/version_strings 2>/dev/null | tr '\n' '.')\x20:$(TARGET)"'
endif

# Always needed objects for Zephyr targets
OBJECTS += target_main.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_process.$(OBJ)
OBJECTS += target_time.$(OBJ)

# Optional objects depending on platform build
ifdef EHS_UART_SUPPORT
ifneq ($(EHS_UART_SUPPORT),none)
OBJECTS += target_uart.${OBJ}
endif
endif

ifdef EHS_GUI_SUPPORT
ifneq ($(EHS_GUI_SUPPORT),none)
OBJECTS += target_display.$(OBJ)
endif
endif
