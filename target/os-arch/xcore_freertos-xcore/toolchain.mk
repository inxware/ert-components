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
# Toolchain binary selection — HOST-style pattern.
#
# The Docker image installs xcc (and its sibling tools: xcc2clang, xmap, …)
# and adds the XTC bin directory to PATH via the Dockerfile ENV instruction.
# We therefore use bare binary names here, just as gnu_ALL does when
# TOOLCHAIN_PATH=HOST, and let PATH do the lookup.
#
# XMOS_TOOL_PATH is used only to:
#   1. Add the XTC bin directory to PATH as a safety net (handles stale images
#      built before the Dockerfile ENV PATH was present).
#   2. Construct the absolute path to the board XN file for the linker.
#
# CC_OVERRIDE / LINK_OVERRIDE remain available for the rare case where a
# developer needs to point at a different xcc installation.
# -----------------------------------------------------------------------------

ifndef CC_OVERRIDE
    CC_OVERRIDE := xcc
endif
export CC   := $(CC_OVERRIDE)
export CPP  := $(CC_OVERRIDE)

ifndef LINK_OVERRIDE
    export LINK := $(CC)
else
    export LINK := $(LINK_OVERRIDE)
endif

AS := $(CC_OVERRIDE)

# xcc is a wrapper script in bin/ that calls helper tools (xcc2clang, xmap, …)
# from libexec/ by bare name.  Both directories must be on PATH.
ifdef XMOS_TOOL_PATH
    export PATH := $(XMOS_TOOL_PATH)/bin:$(XMOS_TOOL_PATH)/libexec:$(PATH)
else
    $(warning XMOS_TOOL_PATH is not set.)
    $(warning Set XMOS_TOOL_PATH to your XTC Tools root, e.g. /opt/XMOS/XTC/15.3.1)
    $(warning xcc and its helper tools must be on PATH for the build to succeed.)
endif

# -----------------------------------------------------------------------------
# Board target — set XMOS_BOARD_TARGET in platform config.mk.
#
# xcc's -target= flag only accepts registered board names, not file paths.
# Passing the XN file directly on the command line is the correct approach:
#   - Compilation (.c → .o): -march=xs3a is sufficient; XN file not needed.
#   - Linking (.o → .xe):    XN file is passed as a positional argument
#                             (not via any flag) after the object files.
#
# XTC Tools lays boards out as:
#   $(XMOS_TOOL_PATH)/targets/<BOARD>/<BOARD>.xn
# -----------------------------------------------------------------------------

ifndef XMOS_BOARD_TARGET
    $(warning XMOS_BOARD_TARGET is not set. Set it in your platform config.mk.)
    $(warning Example: XMOS_BOARD_TARGET=XCORE-AI-EXPLORER)
endif

ifdef XMOS_BOARD_TARGET
    ifdef XMOS_TOOL_PATH
        _XMOS_XN_FILE := $(XMOS_TOOL_PATH)/targets/$(XMOS_BOARD_TARGET)/$(XMOS_BOARD_TARGET).xn
    endif
endif

# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------

# XS3 architecture (xcore.ai). Use -march=xs2a for older xCORE-200 targets.
# No board target flag is needed for .c compilation; -march=xs3a is sufficient.
CFLAGS += -march=xs3a

# XMOS C library headers live in target/include/ (not usr/include/ as a
# standard sysroot would expect).  Compiler built-in headers (stddef.h etc.)
# are in target/include/clang/ rather than being auto-detected from the
# xcc2clang binary location.  Both paths must be added explicitly.
#
# NOTE: These are appended to XTC_SYS_INC rather than CFLAGS directly.
# They must come AFTER the EHS user -I paths (INC_DIRS → INC) in the final
# command line, otherwise XTC system headers (e.g. timer.h for the hardware
# timer peripheral) shadow identically-named EHS headers such as
# Common/KAPI/timer.h.  See the INC/CFLAGS assembly block below.
ifdef XMOS_TOOL_PATH
    XTC_SYS_INC += -I$(XMOS_TOOL_PATH)/target/include
    XTC_SYS_INC += -I$(XMOS_TOOL_PATH)/target/include/clang
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
# XTC system headers (XTC_SYS_INC) are appended last so that EHS headers with
# the same name (e.g. Common/KAPI/timer.h) are found before the XTC ones.
INC    += $(foreach i,$(INC_DIRS),-I$i)
CFLAGS += -c $(INC) $(XTC_SYS_INC)
CFLAGS += $(foreach i,$(DEFS),-D$i)

# -----------------------------------------------------------------------------
# Linker flags
# -----------------------------------------------------------------------------

# XN board description file — passed as a positional argument to the linker.
# The link rule is: $(LINK) $(LD_SWITCHES) $(OBJECTS) $(LNKFLAGS)
# so the XN file appears immediately after the object files, before -l flags.
ifdef _XMOS_XN_FILE
    LNKFLAGS += $(_XMOS_XN_FILE)
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
# Output format — two-phase build
#
# Phase 1 (make all / make all_docker):
#   xcc compiles .c → .o files as normal.  Instead of linking to .xe, the
#   objects are archived into a static library using xar (the XMOS archiver).
#   This only requires the SDK *headers* (in target_libs/include/); the SDK
#   .a files and the XN board file are NOT needed at this stage.
#
# Phase 2 (make targetenv_xmos_docker):
#   targetenv_xmos.sh runs inside the XMOS Docker container and invokes a
#   minimal xcommon_cmake wrapper app (contrib/xmos-sdk/xcore-ehs-app/) that
#   links the Phase 1 .a against fwk_rtos, fwk_io, etc. and the XN board file
#   to produce the final ehs.xe firmware image.  This is pure xcommon_cmake
#   — no cmake hacks required.
#
# xar is the XMOS archiver (part of XTC Tools), analogous to GNU ar.
# Syntax: xar rcs output.a input1.o input2.o ...
# The link rule in the Makefile is: $(LINK) $(LD_SWITCHES) $(OBJECTS) $(LNKFLAGS)
# so LD_SWITCHES carries "rcs <output>" and LNKFLAGS is left empty.
# -----------------------------------------------------------------------------

AR    := xmosar
LINK  := $(AR)
EXE   := a
OBJ   = o
FINAL := a

# Clear linker flags — not needed for archiving.
# Use = (deferred) for LD_SWITCHES so TARGET_NAME is resolved at recipe time.
LD_SWITCHES = rcs $(TARGET_NAME).$(FINAL)
LNKFLAGS   :=
