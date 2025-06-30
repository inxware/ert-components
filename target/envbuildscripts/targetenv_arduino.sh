#!/bin/bash

# creating the arduino final binaries into the target/envbuildscripts/ directory along with all the others

####################################################################################################
# This file script will build the ert source using the docker file available or if not will use
# the host's make system.
####################################################################################################

set -e

#If this is called as a docker command we wont have the exported environment variables, so read them from the paramaters
if [ "${TARGET}" = "" ]; then
    echo "Setting TARGET to ${TARGET}"
    export TARGET=$1
fi
TARGET_ROOT=../TARGET_TREES/ehs_env-${TARGET}
# uncomment for verbose option
#ERT_ARDUINO_VERBOSE=--verbose
ERT_ARDUINO_PROJECT=eRT_Debug
#ERT_ARDUINO_PROJECT=eRT_Release
ERT_ARDUINO_PROJECT_PATH=${TARGET_ROOT}/projects/${ERT_ARDUINO_PROJECT}
ERT_ARDUINO_LIB_DIR=${TARGET_ROOT}/libraries
ERT_ARDUINO_BIN=${TARGET_ROOT}/bin

arduino-cli compile --libraries ${ERT_ARDUINO_LIB_DIR} --fqbn arduino:mbed_nano:nanorp2040connect --output-dir ${ERT_ARDUINO_BIN} ${ERT_ARDUINO_VERBOSE} ${ERT_ARDUINO_PROJECT_PATH}

if [[ "${TARGET}" == *_library ]]; then

echo "====================== PACKAGE A LIBRARY ========================="
# package arduino library and zip it (better for Windows)
ARDUINO_LIB_DIR=${TARGET_ROOT}/libraries
pushd ${ARDUINO_LIB_DIR}
zip -r ../bin/eRT_Package.zip *
popd
echo "=================================================================="
else

# copy eRT flashable binaries to a standard name
cp ${ERT_ARDUINO_BIN}/${ERT_ARDUINO_PROJECT}.ino.uf2 ${ERT_ARDUINO_BIN}/ehs.uf2
# if exists copy flash nuke file for people to be able erasing the flash
ERT_FLASH_NUKE_FILE_DIR=../ert-build-support/toolchains/x86_64/rp2040tools
test -f ${ERT_FLASH_NUKE_FILE_DIR}/flash_nuke.uf2 && cp ${ERT_FLASH_NUKE_FILE_DIR}/flash_nuke.uf2 ${ERT_ARDUINO_BIN}/flash_nuke.uf2

fi

if [ "${FLASH_BOARD}" = "1" ]; then
echo "Flashing ..."
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:mbed_nano:nanorp2040connect --verbose ${ERT_ARDUINO_PROJECT_PATH}
fi

echo "---------------------------------------------------------------------------------------------------------------------------"
echo "All Done!"
