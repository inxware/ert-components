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
# Use the exported EHS_KERNEL_DIR rather than recomposing the path: it is the
# same directory the link used, so the archive copied here cannot differ from
# the one the binary was built against.
ARDUINO_KERNEL_LIB="${EHS_KERNEL_DIR}libehs_ehrt1.a"
if [ ! -f "${ARDUINO_KERNEL_LIB}" ]; then
    echo "ERROR: kernel archive not found: ${ARDUINO_KERNEL_LIB}" >&2
    echo "       Check out ../ert-kernels, or set EHS_KERNEL_BASE." >&2
    exit 1
fi
cp "${ARDUINO_KERNEL_LIB}" "${ARDUINO_LIB_DIR}/libeRT_Kernel.a"

else

echo "Arduino is not built as a library."

fi

echo "DONE!"

echo "###############################################################"