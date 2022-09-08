#!/bin/bash

# creating the esp final binaries into the target/envbuildscripts/ directory along with all the others

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd
echo ""
echo "***** targetenv for ($SPECIFIC_TARGET) *****"
echo ""

#Set path to the python and toolchain.
export TEMP_PWD=${PWD}
export TOOLCHAIN_PATH=$TEMP_PWD/../ert-build-support/toolchains/x86_64/
_PATH="${TOOLCHAIN_PATH}/bin"
export IDF_PYTHON_ENV_PATH="${TOOLCHAIN_PATH}/espressif-4.4.1/python_env/venv/bin"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/openocd-esp32/v0.11.0-esp32-20211220/openocd-esp32/bin"
_PATH="${_PATH:+${_PATH}:}${IDF_PYTHON_ENV_PATH}"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/esptool_py/esptool"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/espcoredump"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/partition_table"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/app_update"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.1/tools/additional_tools"
export PATH="${_PATH}${PATH:+:${PATH}}"

#build and copy supportive binaries
#source ../ert-contrib-middleware/inx_build_scripts/source-scripts/inx-xbuilder-source-me-espidf.sh

#build_bootloader esp-idf -4.4.1 bootloader 
#build_partition_table esp-idf -4.4.1 partition_table

# build .bin file from elf file 

echo "###Building .bin file for esp32 from .elf file###"
$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py --chip esp32 elf2image --flash_mode "dio" --flash_freq "40m" --flash_size "4MB" --elf-sha256-offset 0xb0 ehs_esp32_freertos-xtensor-base.elf -o $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.bin
echo "###Copied ehs.bin file generated in $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/###"

echo "###copying the pre build bootloader binary into TARGET_TREES ###"
test -f "$PWD/../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/bootloader/bootloader.bin" && cp $PWD/../ert-contrib-middleware/target_libs/xtensa-esp32_freertos-xtensa-esp32-elf-4.4.1/build/lib/bootloader.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/


echo "###copying the pre build partion table binary into TARGET_TREES ###"
test -f "$PWD/../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/build/partitions_singleapp.bin" && cp $PWD/../ert-contrib-middleware/target_libs/xtensa-esp32_freertos-xtensa-esp32-elf-4.4.1/build/lib/partitions_singleapp.bin $PWD/../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/

