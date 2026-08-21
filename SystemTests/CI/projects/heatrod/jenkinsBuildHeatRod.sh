#!/bin/bash
if [[ -z "${1}" ]]; then
    echo "First argument should be the build variant. e.g nxp_arm_heatrod_hri_release"
fi
variant="${1}"

echo "TEST_NAME=${TEST_NAME} TEST_PATH=${TEST_PATH} variant=${variant}"
cd ${TEST_PATH}
./configure ${variant}
make prepdeps
make clean
CFLAGS=-DCONFIG_FIRMWARE_VERSION=\\\"${variant}\\\" make -j 4

#flash the devices for this test
./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugClientFlash.sh