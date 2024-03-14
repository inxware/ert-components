#!/bin/bash
# Note this logic is a duplcate of what we should find in ./target/platform/platform.mk

echo "_____________________________________________________________________________________________________________________________________________________"
echo -n "TOOLCHAIN PATH: ${EHS_CORE_SUPPORT_BASE}/toolchains/${TOOLCHAIN_PATH}"
test -d "${EHS_CORE_SUPPORT_BASE}/toolchains/${TOOLCHAIN_PATH}" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;33m -WARNING: MISSING\033[0m"
echo -n "EHS_BUILD_SYSROOT PATH: ${EHS_BUILD_SYSROOT}"
test -d "${EHS_BUILD_SYSROOT}" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;33m -WARNING: MISSING\033[0m"
echo "_____________________________________________________________________________________________________________________________________________________"
echo -n "EHS KERNEL :${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/kernel/libehs.a"
test -f "${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/kernel/libehs.a" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;31m -ERROR: N/A\033[0m"


if [ "${EHS_HOST_DEBIAN_BUILD}" != "" ]; then
    echo "HOST_BUILD is set. Using build host and contributed middleware in ert-contrib-middleware"
    echo -n "KERNEL:${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/kernel/"
    test -d "${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/kernel/" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;34m -INFO: N/A\033[0m"
else
  # Add paths the ert-build-support's LIBC
   if [ "${EHS_CLIB_OVERRIDE_PATH}" != "" ]; then
    echo "A libc override path has been set, to look for libraries in the ert-build-support"
    echo "TODO!!! fix me in ./target/envuildscripts/buildenv_checkpaths.sh"
#       $(info == EHS_CLIB_OVERRIDE_PATH is set, using the override path:)
#       $(info == [../ert-build-support/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/])
#       export INC_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/include/
#       export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/build/lib/
#       export LIB_DIRS+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/kernel/
      # done properly gnu toolchain.mk export LD_LIBRARY_PATH+=$(EHS_CORE_SUPPORT_BASE)/support_libs/target_libs/$(EHS_CLIB_OVERRIDE_PATH)/lib/
   else
       echo -n "BUILD SUPP :${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/build/"
       test -d "${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/build/" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;34m -INFO: N/A\033[0m"
   fi
fi

#echo "ERT-CONTRIB-MIDDLEWARE: Using the following contributed middleware library path"
echo -n "CONTRIB INC: ${EHS_COMPONENT_SUPPORT_INCLUDE}"
test -d "${EHS_COMPONENT_SUPPORT_INCLUDE}" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;33m -WARNING: MISSING\033[0m"
echo -n "CONTRIB LIB: ${EHS_COMPONENT_SUPPORT_LIBS}"
test -d "${EHS_COMPONENT_SUPPORT_LIBS}" && echo -e "\033[0;32m -EXISTS\033[0m" || echo -e "\033[0;33m -WARNING: MISSING\033[0m"
echo -e "_____________________________________________________________________________________________________________________________________________________"
