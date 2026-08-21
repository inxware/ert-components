#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------

# HW-independent QNX Neutrino build environment parameters.
# Included by architecture-specific target.mk files (e.g. qnx-arm64/target.mk).
#
# Build chain:
#   qnx-<arch>/target.mk
#     → qnx_ALL/target.mk          (this file: OS objects + gnu_ALL)
#       → gnu_ALL/target.mk        (target_main, target_file, target_process, target_math)
#
# Note: gnu_ALL is reused because QNX provides the same POSIX file/process
# APIs as Linux (fopen/fread, fork/exec, main() entry point).  Only the
# OS-specific layer (targetos_init, target_time, target_sys_stat) differs
# and those live here in qnx_ALL.

include $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx_ALL/config.mk

INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx_ALL/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/platform

VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/qnx_ALL/

# Pull in gnu_ALL support objects (target_main, target_file, target_process, target_math).
# gnu_ALL is a generic GCC/Clang toolchain shim; its POSIX file/process
# APIs compile without modification on QNX.
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk

# QNX-specific OS runtime objects
OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_sys_stat.$(OBJ)
