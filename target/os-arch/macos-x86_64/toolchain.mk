#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

# macOS x86_64 toolchain.
# Uses Apple clang from the Xcode Command Line Tools (TOOLCHAIN_NAME=HOST).
# libm and libpthread are part of libSystem on macOS — no separate -l flags needed.

include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
