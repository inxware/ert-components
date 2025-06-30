#!/bin/bash

####################################################################################################
# This file script will run everything that is needed before building traget.
####################################################################################################

set -e

TARGET=$1

echo "************ targetenv littlefs ************"
mkdir -p ${PWD}/../TARGET_TREES/ehs_env-${TARGET}
pushd ${PWD}/../TARGET_TREES/ehs_env-${TARGET}
if [[ "$TARGET" == *"esp32s3_freertos-xtensa"* ]]; then
    find -name .gitignore -delete
    rm -rf devman/plugins
    rm -rf devman/pubdata
    /opt/python_env/bin/littlefs-python create -v --block-size 4096 --fs-size 512000 --name-max 64 --image bin/app_data.bin appdata
    pushd bin
    printf "#include \"target_data_bin.h\"\n\nconst %s\n" "$(xxd -i -C app_data.bin)" > ${PWD}/../../../ert-components/target/os-arch/esp32s3_freertos-xtensa/target_data_bin.c
    popd
else
    echo "!!! WARNING !!! Target ${TARGET} doesn't support 'targetenv littlefs' feature"
fi
popd
