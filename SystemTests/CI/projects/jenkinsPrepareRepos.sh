#!/bin/bash
set -x
echo "make sure ert-build-support is up to date"
cd ${TEST_PATH}
BUILD_SUPPORT_PATH="${TEST_PATH}/../ert-build-support"
if [ ! -d "${BUILD_SUPPORT_PATH}" ]; then
    ln -s ${TEST_PATH}/../EHS-build-support ${BUILD_SUPPORT_PATH}
fi
cd ${BUILD_SUPPORT_PATH}
git pull origin master

echo "make sure ert-contrib-middleware is up to date"
COMP_LIB_PATH="${TEST_PATH}/../comp-lib-support"
if [ ! -d "${COMP_LIB_PATH}" ]; then
    cd ${TEST_PATH}/../
    git clone repo-user@dev.inx-systems.net:/home/inx-data/data/Repos/comp-lib-support.git
fi
ERT_CONTRIB_PATH="${TEST_PATH}/../ert-contrib-middleware"
if [ ! -d "${ERT_CONTRIB_PATH}" ]; then
    ln -s ${COMP_LIB_PATH} ${ERT_CONTRIB_PATH}
fi
cd ${ERT_CONTRIB_PATH}
git pull origin master

echo "make sure ert-components is up to date"
EHS_PATH="${TEST_PATH}/../EHS"
if [ ! -d "${EHS_PATH}" ]; then
    cd ${TEST_PATH}/../
    git clone repo-user@dev.inx-systems.net:/home/inx-data/data/Repos/ert-components.git
fi
ERT_COMPONENTS_PATH="${TEST_PATH}/../ert-components"
if [ ! -d "${ERT_COMPONENTS_PATH}" ]; then
    ln -s ${EHS_PATH} ${ERT_COMPONENTS_PATH}
fi
cd ${ERT_COMPONENTS_PATH}
git pull origin master

echo "make sure EHS-kernel is up to date"
EHS_KERNEL_PATH=${TEST_PATH}/../EHS-kernel
if [ ! -d "${EHS_KERNEL_PATH}" ]; then
    cd ${TEST_PATH}/../
    git clone repo-user@dev.inx-systems.net:/home/inx-data/data/Repos/EHS-kernel.git
fi
cd ${EHS_KERNEL_PATH}
git pull origin master

cd ${TEST_PATH}/../
if [ -e DevmanSecurity ]; then
    echo "already have devman security"
else
    git clone ssh://repo-user@dev.inx-systems.net:8822/home/inx-data/data/Repos/DevmanSecurity.git
fi
cd DevmanSecurity
git checkout master
git pull origin master