#!/bin/bash
set -x
if [ -z "${TEST_PATH}" ]; then
    TEST_PATH=`pwd`
fi

${TEST_PATH}/SystemTests/CI/projects/jenkinsPrepareRepos.sh
EHS_KERNEL_PATH=${TEST_PATH}/../EHS-kernel
cd ${EHS_KERNEL_PATH}
./configure linux_android_arm
make clean
make

SIGNED_APK_PATH=${TEST_PATH}/../TARGET_TREES/ehs_env-linux_android_arm_p64_a6_player-testing/bin/player.apk
if [ -e ${SIGNED_APK_PATH} ]; then
    rm ${SIGNED_APK_PATH}
fi

cd ${TEST_PATH}
./configure linux_android_arm_p64_a6_player-testing-ci
make clean
make prepdeps 
make all
make targetenv
mkdir ../TARGET_TREES/ehs_env-linux_android_arm_p64_a6_player-testing-ci/sysdata
TIME_STAMP=`date +%F-%Hh%Mm%Ss`
echo "${TIME_STAMP}-ci" > ../TARGET_TREES/ehs_env-linux_android_arm_p64_a6_player-testing-ci/sysdata/version.nfo
make targetenv_apk

mkdir -p ${TEST_PATH}/SystemTests/CI/projects/player/test-results/
if [ -e ${SIGNED_APK_PATH} ]; then
    cp ${TEST_PATH}/SystemTests/CI/projects/player/build-passed.xml ${TEST_PATH}/SystemTests/CI/projects/player/test-results/TEST-build.xml
else
    cp ${TEST_PATH}/SystemTests/CI/projects/player/build-failed.xml ${TEST_PATH}/SystemTests/CI/projects/player/test-results/TEST-build.xml
fi

echo "now trying to upload APK for deployment"
make upload_ehs_sys_patch

echo "now trying to deploy to devices"
${TEST_PATH}/SystemTests/CI/projects/player/deployToDevices.sh