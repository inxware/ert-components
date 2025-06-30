#!/bin/bash

set -e

TARGET=$1
TARGET_ROOT=../TARGET_TREES/ehs_env-${TARGET}

echo "###############################################################"
echo "######################## ARDUINO HACK #########################"
echo "###############################################################"

if [ -n "${EHS_PLUGIN_LIBRARY_DEPENDENCY}" ]; then

ARDUINO_LIB_DIR_ROOT=${TARGET_ROOT}/libraries/eRT
ARDUINO_LIB_DIR=${ARDUINO_LIB_DIR_ROOT}/src/${EHS_PLUGIN_LIBRARY_DEPENDENCY}
echo "Create arduino static library (${ARDUINO_LIB_DIR})"
mkdir -p ${ARDUINO_LIB_DIR}
# copy eRT static libary to the arduino lib structure
cp ${TARGET_ROOT}/bin/ehs.a ${ARDUINO_LIB_DIR}/libeRT.a
# copy eRT Kernel static libary to the arduino lib structure
ARDUINO_KERNEL_LIB=
if [ -n "${EHS_CLIB_OVERRIDE_PATH}" ]; then
    ARDUINO_KERNEL_LIB="${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_CLIB_OVERRIDE_PATH}/kernel/libehs_ehrt1.a"
else
    ARDUINO_KERNEL_LIB="${EHS_CORE_SUPPORT_BASE}/support_libs/target_libs/${EHS_GNU_OS_ARCH}${EHS_SPECIAL_CLIB_EXT}/kernel/libehs_ehrt1.a"
fi
cp ${ARDUINO_KERNEL_LIB} ${ARDUINO_LIB_DIR}/libeRT_Kernel.a

else

echo "Arduino is not built as a library."

fi

echo "DONE!"

echo "###############################################################"