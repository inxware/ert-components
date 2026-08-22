#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# macOS x86_64 target build fragment.
# Re-uses linux_ALL source files (target_time.c, targetos_init.c, etc.); those
# files guard macOS-incompatible code paths with #ifdef EHS_MACOS.

include $(EHS_TARGETS_ROOT_PATH)/os-arch/macos-x86_64/config.mk

INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/macos-x86_64/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/
INC_DIRS += $(EHS_TARGETS_ROOT_PATH)/platform

# macos-x86_64 directory first so any future override files take priority over linux_ALL
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/macos-x86_64/
VPATH += $(EHS_TARGETS_ROOT_PATH)/os-arch/linux_ALL/

# Shared sources from linux_ALL — all guarded with EHS_MACOS / EHS_BSD where needed
OBJECTS += target_time.$(OBJ)
OBJECTS += targetos_init.$(OBJ)
OBJECTS += target_display.$(OBJ)
OBJECTS += target_sys_stat.$(OBJ)

# macOS: libarchive is available as a keg-only Homebrew formula.
# The brew symlink /usr/local/opt/libarchive is stable across upgrades.
INC_DIRS += /usr/local/opt/libarchive/include
LNKFLAGS += -L/usr/local/opt/libarchive/lib

# Include gnu_ALL last: adds gnu_ALL/ to VPATH (provides target_specific.h) and
# contributes the mandatory GNU objects (target_file, target_process, target_main,
# target_math, target_app_lifecycle) and standard GNU libs (z, archive, idn2).
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/target.mk
