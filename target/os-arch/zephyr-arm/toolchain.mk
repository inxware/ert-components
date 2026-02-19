#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)

# Zephyr SDK ARM toolchain configuration
# This follows the same pattern as esp32s3_freertos-xtensa/toolchain_idf_5_1_x.mk

# Setup the toolchain path - Zephyr SDK provides arm-zephyr-eabi-* tools
ifndef CC_OVERRIDE
   CC_OVERRIDE:=arm-zephyr-eabi-gcc
endif

ifndef AS_OVERRIDE
   AS_OVERRIDE:=arm-zephyr-eabi-as
endif

ifndef LINK_OVERRIDE
    LINK_OVERRIDE:=arm-zephyr-eabi-gcc
endif

# Import Zephyr build exports (compiler flags, includes, lib paths)
# ZEPHYR_BUILD_DIR must point to a Zephyr west build output directory
# that contains Makefile.export (generated via CMake outputexports)
ifdef ZEPHYR_BUILD_DIR
-include $(ZEPHYR_BUILD_DIR)/Makefile.export
# Append Zephyr-exported flags if Makefile.export was found
ifdef ZEPHYR_CFLAGS
CFLAGS += $(ZEPHYR_CFLAGS)
endif
ifdef ZEPHYR_INCLUDE_DIRS
INC_DIRS += $(ZEPHYR_INCLUDE_DIRS)
endif
ifdef ZEPHYR_LIB_DIRS
LIB_DIRS += $(ZEPHYR_LIB_DIRS)
endif
ifdef ZEPHYR_LIBS
LNKFLAGS += $(ZEPHYR_LIBS)
endif
endif

# MCU-specific flags (overridable by platform config.mk via ZEPHYR_CPU_FLAGS)
# Default: Cortex-M33 (e.g. nRF5340, STM32U5, etc.)
ifndef ZEPHYR_CPU_FLAGS
ZEPHYR_CPU_FLAGS = -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
endif
CFLAGS += $(ZEPHYR_CPU_FLAGS)

# Common compiler flags for Zephyr targets
ifndef ZEPHYR_DEBUG_BUILD
CFLAGS += -std=gnu17 -Os -ggdb -ffunction-sections -fdata-sections -nostdlib -Wall
CFLAGS += -DNDEBUG
else
CFLAGS += -std=gnu17 -Og -ggdb -ffunction-sections -fdata-sections -nostdlib -Wall
endif
CFLAGS += -MMD -MP
CFLAGS += -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS
CFLAGS += -DZEPHYR_RTOS=1

# Linker flags
LNKFLAGS += -nostdlib -Wl,--gc-sections -Wl,--warn-common

# Import linker scripts from Zephyr build if available
ifdef ZEPHYR_BUILD_DIR
LNKFLAGS += -T $(ZEPHYR_BUILD_DIR)/zephyr/linker.cmd
endif

LDFLAGS = -static
EXE=elf

EHS_MCU_TARGET=yes

# Use the usual gcc/clang tool chain config
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
