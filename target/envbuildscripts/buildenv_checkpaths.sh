#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# Note this logic is a duplcate of what we should find in ./target/platform/platform.mk

source ./scripts/build-function-library/colour.sh

if [ "${ERT_SODL_VERSION}" = "1" ]; then
    EHS_KERNEL_LIB=libehs_ehrt1.a
else
    EHS_KERNEL_LIB=libehs.a
fi

echo "${TXT_FG_GREY}--${TXT_FG_CYAN}TOOLCHAIN${TXT_FG_GREY}---------------------------------------------------------"
if [ "${TOOLCHAIN_PATH}" = "HOST" ] || [ "${TOOLCHAIN_NAME}" = "HOST" ]; then
    echo "${TXT_FG_BLUE}TOOLCHAIN:${TXT_FG} Using host system compiler"
    echo "${TXT_FG_BLUE}EHS_BUILD_SYSROOT:${TXT_FG} Using host system sysroot"
else
    echo -n "${TXT_FG_BLUE}TOOLCHAIN PATH:${TXT_FG} ${EHS_CORE_SUPPORT_BASE}/toolchains/${TOOLCHAIN_PATH} - "
    test -d "${EHS_CORE_SUPPORT_BASE}/toolchains/${TOOLCHAIN_PATH}" && echo "${TXT_FG_GREEN}EXISTS" || warn "MISSING"
    echo -n "${TXT_FG_BLUE}EHS_BUILD_SYSROOT PATH:${TXT_FG} ${EHS_BUILD_SYSROOT} - "
    test -d "${EHS_BUILD_SYSROOT}" && echo "${TXT_FG_GREEN}EXISTS" || warn "MISSING"
fi
echo
echo "${TXT_FG_GREY}--${TXT_FG_CYAN}DEPENDENCIES${TXT_FG_GREY}----------------------------------------------------"
# Use the resolved EHS_KERNEL_DIR exported by platform.mk rather than composing
# the path again here. This file used to duplicate the expression, so when the
# kernel archives moved to ../ert-kernels it kept reporting the old location —
# and check_target_keys.sh parses this line, so the assertion would have been
# checking a path the build no longer uses.
echo -n "${TXT_FG_BLUE}EHS KERNEL:${TXT_FG} ${EHS_KERNEL_DIR}${EHS_KERNEL_LIB} - "
test -f "${EHS_KERNEL_DIR}${EHS_KERNEL_LIB}" && echo "${TXT_FG_GREEN}EXISTS" || err "N/A"


if [ "${EHS_HOST_DEBIAN_BUILD}" != "" ]; then
    echo "HOST_BUILD is set. Using build host and contributed middleware in ert-contrib-middleware"
    # Kernels live in ert-kernels now; EHS_KERNEL_DIR is exported by platform.mk.
    echo -n "${TXT_FG_BLUE}KERNEL:${TXT_FG}    ${EHS_KERNEL_DIR} - "
    test -d "${EHS_KERNEL_DIR}" && echo "${TXT_FG_GREEN}EXISTS" || echo "${TXT_FG_WHITE}INFO: N/A"
else
    # Add paths the ert-build-support's LIBC
    if [ "${EHS_CLIB_OVERRIDE_PATH}" != "" ]; then
        echo "A libc override path has been set, to look for libraries in the ert-build-support"
        warn "TODO!!! fix me in ./target/envuildscripts/buildenv_checkpaths.sh"
        # $(info == EHS_CLIB_OVERRIDE_PATH is set, using the override path:)
        # $(info == [../ert-build-support/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/])
        # export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
        # export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
        # export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/kernel/
        # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/lib/
    else
       echo -n "${TXT_FG_BLUE}BUILD SUPP:${TXT_FG} ${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}/build/ - "
       test -d "${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}/build/" && echo "${TXT_FG_GREEN}EXISTS" || echo "${TXT_FG_WHITE}INFO: N/A"
    fi
fi

#echo "ERT-CONTRIB-MIDDLEWARE: Using the following contributed middleware library path"
echo -n "${TXT_FG_BLUE}CONTRIB INC:${TXT_FG} ${EHS_COMPONENT_SUPPORT_INCLUDE} - "
test -d "${EHS_COMPONENT_SUPPORT_INCLUDE}" && echo "${TXT_FG_GREEN}EXISTS" || warn "MISSING"
echo -n "${TXT_FG_BLUE}CONTRIB LIB:${TXT_FG} ${EHS_COMPONENT_SUPPORT_LIBS} - "
test -d "${EHS_COMPONENT_SUPPORT_LIBS}" && echo "${TXT_FG_GREEN}EXISTS" || warn "MISSING"
echo "${TXT_FG_GREY}--------------------------------------------------------------------"
echo
exit 0
