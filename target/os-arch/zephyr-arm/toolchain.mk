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

# ---------------------------------------------------------------------------
# Toolchain path resolution — current state and future options
#
# The arm-zephyr-eabi-* binaries below are bare names with no path prefix.
# They are resolved via $PATH at build time.  This works today because the
# Zephyr Docker image (ghcr.io/zephyrproject-rtos/ci:v0.27.4) places the
# Zephyr SDK under /opt/toolchains/zephyr-sdk-<ver>/arm-zephyr-eabi/bin and
# adds that directory to $PATH.  Outside the Docker container the tools will
# not be found unless the user has set up $PATH manually.
#
# Three realistic installation scenarios to support in future:
#
#   1. Docker / /opt/ install  — current behaviour; tools are on $PATH
#      (equivalent to TOOLCHAIN_NAME=HOST for other targets).
#
#   2. ert-build-support tree  — place the SDK under
#      ../ert-build-support/toolchains/x86_64/arm-zephyr-eabi/
#      and set TOOLCHAIN_NAME=arm-zephyr-eabi in config.mk.  platform.mk
#      would then construct TOOLCHAIN_PATH and prepend bin/ to CC etc.,
#      exactly as it does for arm-none-eabi and xtensa toolchains.
#
#   3. User-defined path       — e.g. Nordic's nRF Connect SDK installs its
#      own Zephyr SDK at an arbitrary location.  A new make variable such as
#      ERT_ZEPHYR_SDK_PATH could be set in config.mk or the environment to
#      override the bare-name resolution.
#
# TODO: Decide whether we need a new TOOLCHAIN_NAME keyword "OPT" (analogous
# to "HOST") meaning "the toolchain is installed at an absolute /opt/ path on
# this machine".  "HOST" already means "on $PATH" and currently covers the
# Docker case implicitly.  An explicit "OPT" keyword would only add value if
# we want platform.mk to auto-construct the /opt/toolchains/<sdk>/... prefix
# rather than relying on the Docker image's PATH setup.  The cleaner long-term
# option is probably scenario 2 (ert-build-support) or scenario 3
# (ERT_ZEPHYR_SDK_PATH) rather than hardcoding /opt/ semantics into the
# keyword.
# ---------------------------------------------------------------------------

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
CFLAGS += -DEHS_ZEPHYR_RTOS=1

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
