#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
# @author: inx limited

# Makefile fragment for QNX Neutrino on AArch64 (Cortex-A72 / RPi4).
# Called by target/platform/<platform>/config.mk via the build system.
#
# This file adds AArch64-specific INC_DIRS, VPATH, and any objects that
# are unique to this architecture.  All QNX OS layer objects are pulled
# in by qnx_ALL/target.mk (which in turn pulls gnu_ALL/target.mk).

include $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx-arm64/config.mk

INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx-arm64/
VPATH    += $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx-arm64/

# Shared QNX OS layer + gnu_ALL portable objects
include $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx_ALL/target.mk

# Architecture-specific component HAL selections
include $(EHS_TARGET_OS_HW_PATH)/Components/components.mk
