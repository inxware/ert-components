#!/bin/bash
set -x
${TEST_PATH}/SystemTests/CI/projects/jenkinsPrepareRepos.sh
EHS_KERNEL_PATH=${TEST_PATH}/../EHS-kernel
cd ${EHS_KERNEL_PATH}
./configure linux_android_arm
make clean
make
cd ${TEST_PATH}
./configure linux_android_arm_p64_a6_ambifier-sandbox
make clean
make prepdeps 
make all
make targetenv
make targetenv_apk