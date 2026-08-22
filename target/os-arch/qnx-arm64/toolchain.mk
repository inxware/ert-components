#---------------------------------------------------------------
# Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#  Uses $(INC_DIRS), $(TARGET_NAME), $(DEFS)
#  Defines $(CC), $(LINK), $(CFLAGS), $(LNKFLAGS), $(INC), $(LIB), $(EXE), $(OBJ), $(FINAL)

# QNX Neutrino RTOS ARM64 toolchain configuration
# GNU canonical triple: aarch64-unknown-nto-qnx8.0.0

# ---------------------------------------------------------------------------
# Include strategy: gnu_ALL vs. duplicate vs. posix_ALL extraction
#
# gnu_ALL/toolchain.mk is generic enough to reuse directly:
#   - CC/LINK/AS selection via *_OVERRIDE — fully generic
#   - TOOLCHAIN_PATH prefix logic for ert-build-support — generic
#   - Sysroot resolution via EHS_SYSROOT_ABS_PATH_OVERRIDE — generic
#   - Flag assembly (-c, -I, -D, -L, -l, -Wl,-o, -Wl,-E) — portable to QNX ELF
#   - -std=gnu99 — QNX GCC supports this
#
# The only Linux-specific addition is in linux_ALL/toolchain.mk (LIB+=pthread).
# On QNX SDP 8.0, pthreads are integrated into libc — we simply omit
# linux_ALL and handle QNX library differences below.
#
# A posix_ALL intermediate was not extracted: the candidate content (pthread
# semantics) differs between Linux and QNX enough to make a shared base
# misleading. Revisit if a third POSIX OS target (macOS, FreeBSD) is added.
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Toolchain path resolution
#
# Current: TOOLCHAIN_NAME=HOST (set in platform config.mk).
#   Cross-tools are on $PATH inside the Docker build container.
#   The Dockerfile sets:
#     ENV QNX_HOST  /opt/qnx800/host/linux/x86_64
#     ENV QNX_TARGET /opt/qnx800/target/qnx
#   and adds $QNX_HOST/usr/bin to $PATH.
#   The QNX cross-compiler uses $QNX_TARGET at compile/link time to locate
#   its sysroot automatically — no explicit --sysroot flag is required.
#
# Future option: set TOOLCHAIN_NAME=qnx800 and install the SDP into
#   ../ert-build-support/toolchains/x86_64/qnx800/
#   to follow the standard ert-build-support resolution in gnu_ALL.
# ---------------------------------------------------------------------------

# QNX SDP 8.0 cross-compiler for AArch64 little-endian Neutrino
ifndef CC_OVERRIDE
    CC_OVERRIDE := aarch64-unknown-nto-qnx8.0.0-gcc
endif
ifndef LINK_OVERRIDE
    LINK_OVERRIDE := aarch64-unknown-nto-qnx8.0.0-gcc
endif
ifndef AS_OVERRIDE
    AS_OVERRIDE := aarch64-unknown-nto-qnx8.0.0-as
endif

# CPU flags — default Cortex-A72 (Raspberry Pi 4); overridable per platform config.mk
ifndef QNX_CPU_FLAGS
    QNX_CPU_FLAGS := -mcpu=cortex-a72 -march=armv8-a
endif
CFLAGS   += $(QNX_CPU_FLAGS)
CPPFLAGS += $(QNX_CPU_FLAGS)

# QNX Neutrino RTOS identification and eRT platform guard
CFLAGS   += -DEHS_QNX_RTOS=1
CPPFLAGS += -DEHS_QNX_RTOS=1

# BSD socket API lives in libsocket on QNX (not in libc)
LIB += socket

# QNX pthreads: integrated into libc since QNX SDP 7.1 — no -lpthread needed.
# Uncomment if explicitly linking against code that references libpthread.so:
# LIB += pthread

# Use the common GNU toolchain driver infrastructure
include $(EHS_TARGETS_ROOT_PATH)/os-arch/gnu_ALL/toolchain.mk
