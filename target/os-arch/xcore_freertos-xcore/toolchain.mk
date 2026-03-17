#---------------------------------------------------------------
# Copyright (C) 2008-2025 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)
#
# XMOS xcore.ai toolchain configuration.
#
# NOTE: xcc is NOT a self-contained cross-compiler. It requires XMOS_TOOL_PATH to be set
# in the environment, pointing to the XTC Tools installation directory. This is handled by
# the platform Dockerfile and passed through via INX_ERTCOMPONENTS_BUILDENV in
# target/envbuildscripts/target_buildenv_run_command.sh.
#
# Do NOT include gnu_ALL/toolchain.mk — xcc does not follow GNU toolchain conventions.

# -----------------------------------------------------------------------------
# Toolchain binary selection
# xcc serves as both compiler and linker for xcore targets.
# -----------------------------------------------------------------------------

ifndef CC_OVERRIDE
    CC_OVERRIDE:=xcc
endif

ifndef LINK_OVERRIDE
    LINK_OVERRIDE:=xcc
endif

export CC:=$(CC_OVERRIDE)
export CPP:=$(CC_OVERRIDE)
export LINK:=$(LINK_OVERRIDE)

# xcc handles assembly internally — no separate assembler needed
AS:=xcc

# -----------------------------------------------------------------------------
# Validate XMOS_TOOL_PATH is set
# This will be set in the Docker container. For host builds, set it manually.
# -----------------------------------------------------------------------------

ifndef XMOS_TOOL_PATH
    $(warning XMOS_TOOL_PATH is not set. xcc will fail to locate device target files.)
    $(warning Set XMOS_TOOL_PATH to your XTC Tools installation, e.g. /opt/XMOS/XTC/15.3.0)
endif

# -----------------------------------------------------------------------------
# Board target — set XMOS_BOARD_TARGET in platform config.mk
# This is the board name or path to a .xn file, e.g. XCORE-AI-EXPLORER
# -----------------------------------------------------------------------------

ifndef XMOS_BOARD_TARGET
    $(warning XMOS_BOARD_TARGET is not set. Set it in your platform config.mk.)
    $(warning Example: XMOS_BOARD_TARGET=XCORE-AI-EXPLORER)
endif

# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------

# XS3 architecture (xcore.ai). Use -march=xs2a for older xCORE-200 targets.
CFLAGS += -march=xs3a

# Board target definition (may be a board name or path to a .xn file)
ifdef XMOS_BOARD_TARGET
    CFLAGS += -target $(XMOS_BOARD_TARGET)
endif

# Standard C with GNU extensions, optimise for size, keep debug info
CFLAGS += -std=gnu11 -Os -g

# Section-level dead code elimination
CFLAGS += -ffunction-sections -fdata-sections

# Warnings
CFLAGS += -Wall -Wno-unused-function

# Dependency tracking
CFLAGS += -MMD -MP

# Platform identification macros
CFLAGS += -DXMOS_XCORE=1 -DXMOS_XCORE_AI=1

# FreeRTOS SMP tile count (set per-platform if different from default of 1 tile)
ifndef XMOS_FREERTOS_TILES
    XMOS_FREERTOS_TILES=1
endif
CFLAGS += -DXMOS_FREERTOS_TILES=$(XMOS_FREERTOS_TILES)

# -----------------------------------------------------------------------------
# Linker flags
# -----------------------------------------------------------------------------

ifdef XMOS_BOARD_TARGET
    LNKFLAGS += -target $(XMOS_BOARD_TARGET)
endif

# Remove unused sections
LNKFLAGS += -Wl,--gc-sections

# fwk_rtos requires the FreeRTOS SMP kernel
LNKFLAGS += -lfreertos -lrtos_support

# -----------------------------------------------------------------------------
# Output format
# xcc produces .xe (XMOS executable) files, not ELF.
# Use 'xflash' to program the device from the .xe file.
# -----------------------------------------------------------------------------

EXE=xe
OBJ=o
