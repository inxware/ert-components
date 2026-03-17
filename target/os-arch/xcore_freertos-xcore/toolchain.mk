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
# xcc is a compiler driver (LLVM/Clang-based) that accepts standard -I/-L/-l/-D
# flags.  It is NOT a self-contained cross-compiler: it requires XMOS_TOOL_PATH
# to be set so it can find device target (.xn) files, builtins, and toolchain
# libraries.  XMOS_TOOL_PATH is baked into the Docker image as an ENV variable
# by the platform Dockerfile, so it is always available inside the container.
#
# Do NOT include gnu_ALL/toolchain.mk — xcc does not follow GNU toolchain
# conventions for sysroot selection or library layout.  The INC/CFLAGS/LNKFLAGS
# expansions that gnu_ALL normally performs are reproduced explicitly below.

# -----------------------------------------------------------------------------
# Toolchain binary selection — follows gnu_ALL CC_OVERRIDE / LINK_OVERRIDE pattern.
#
# CC_OVERRIDE / LINK_OVERRIDE set the bare binary name (default: xcc).
# XMOS_TOOL_PATH then prepends the absolute path — analogous to how gnu_ALL uses
# $(EHS_CORE_SUPPORT_BASE)/toolchains/$(TOOLCHAIN_PATH)/bin/ for ert-build-support
# toolchains.  XMOS_TOOL_PATH is baked into the Docker image ENV; on a host build
# the developer must set it manually (or have xcc on PATH and leave it unset).
# -----------------------------------------------------------------------------

ifndef CC_OVERRIDE
    CC_OVERRIDE := xcc
endif
export CC  := $(CC_OVERRIDE)
export CPP := $(CC_OVERRIDE)

ifndef LINK_OVERRIDE
    export LINK := $(CC)
else
    export LINK := $(LINK_OVERRIDE)
endif

AS := $(CC_OVERRIDE)

# Prepend absolute path from XMOS_TOOL_PATH (analogous to TOOLCHAIN_PATH in gnu_ALL).
# Without XMOS_TOOL_PATH, the bare binary name is used and xcc must be on PATH.
ifdef XMOS_TOOL_PATH
    CC   := $(XMOS_TOOL_PATH)/bin/$(CC)
    CPP  := $(XMOS_TOOL_PATH)/bin/$(CPP)
    LINK := $(XMOS_TOOL_PATH)/bin/$(LINK)
    AS   := $(XMOS_TOOL_PATH)/bin/$(AS)
else
    $(warning XMOS_TOOL_PATH is not set — falling back to PATH-based xcc lookup.)
    $(warning Set XMOS_TOOL_PATH to your XTC Tools installation, e.g. /opt/XMOS/XTC/15.3.1)
endif

# -----------------------------------------------------------------------------
# Board target — set XMOS_BOARD_TARGET in platform config.mk.
#
# xcc's built-in board-name lookup requires it to locate its target database
# via XMOS_TOOL_PATH.  To avoid relying on that lookup, we pass the explicit
# path to the XN file instead.  XTC Tools lays boards out as:
#   $(XMOS_TOOL_PATH)/targets/<BOARD>/<BOARD>.xn
#
# If XMOS_TOOL_PATH is not set (host build, xcc on PATH), we fall back to
# the board name alone and rely on xcc finding its own database.
# -----------------------------------------------------------------------------

ifndef XMOS_BOARD_TARGET
    $(warning XMOS_BOARD_TARGET is not set. Set it in your platform config.mk.)
    $(warning Example: XMOS_BOARD_TARGET=XCORE-AI-EXPLORER)
endif

ifdef XMOS_BOARD_TARGET
    ifdef XMOS_TOOL_PATH
        _XMOS_TARGET_FLAG := -target=$(XMOS_TOOL_PATH)/targets/$(XMOS_BOARD_TARGET)/$(XMOS_BOARD_TARGET).xn
    else
        _XMOS_TARGET_FLAG := -target=$(XMOS_BOARD_TARGET)
    endif
endif

# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------

# XS3 architecture (xcore.ai). Use -march=xs2a for older xCORE-200 targets.
CFLAGS += -march=xs3a

ifdef XMOS_BOARD_TARGET
    CFLAGS += $(_XMOS_TARGET_FLAG)
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

# Aggregate include paths from INC_DIRS and preprocessor definitions from DEFS.
# Uses lazy (recursive) expansion so all INC_DIRS and DEFS accumulated later
# (in target.mk, platform config.mk, component makefiles) are included.
INC    += $(foreach i,$(INC_DIRS),-I$i)
CFLAGS += -c $(INC)
CFLAGS += $(foreach i,$(DEFS),-D$i)

# -----------------------------------------------------------------------------
# Linker flags
# -----------------------------------------------------------------------------

# Board target for linker: same explicit XN path as compiler
ifdef XMOS_BOARD_TARGET
    LNKFLAGS += $(_XMOS_TARGET_FLAG)
endif

# Remove unused sections
LNKFLAGS += -Wl,--gc-sections

# Output file name
LNKFLAGS += -o $(TARGET_NAME).$(EXE)

# Library search paths and libraries (populated from LIB_DIRS/LIB in target.mk
# and component makefiles; lazy expansion ensures all additions are captured).
LNKFLAGS += $(foreach i,$(LIB_DIRS),-L$i)
LNKFLAGS += $(foreach i,$(LIB),-l$i)

# -----------------------------------------------------------------------------
# Output format
# xcc produces .xe (XMOS executable) files, not ELF.
# Use 'xflash' to program the device from the .xe file.
# -----------------------------------------------------------------------------

EXE   = xe
OBJ   = o
FINAL = $(EXE)
