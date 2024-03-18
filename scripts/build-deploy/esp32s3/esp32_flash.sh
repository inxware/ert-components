#!/bin/bash
set -e


source TARGET.cfg
export TEMP_PWD=${PWD}
export TOOLCHAIN_PATH=$TEMP_PWD/../ert-build-support/toolchains/x86_64/
_PATH="${TOOLCHAIN_PATH}/bin"
#export IDF_PYTHON_ENV_PATH="${TOOLCHAIN_PATH}/espressif-4.4.4/python_env/venv/bin"
#This is for docker environments.
export IDF_PYTHON_ENV_PATH="/opt/venv/bin"

_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/openocd-esp32/v0.11.0-esp32-20221026/openocd-esp32/bin"
_PATH="${_PATH:+${_PATH}:}${IDF_PYTHON_ENV_PATH}"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/esptool_py/esptool"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/espcoredump"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/partition_table"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/app_update"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/espressif-4.4.4/tools/additional_tools"
export PATH="${_PATH}${PATH:+:${PATH}}"

echo "HAVE YOU RUN make targetenv_esp32 yet???????"

PATH_TO_BINARIES=../TARGET_TREES/ehs_env-$TARGET/bin/

if [ 0 = 1 ]; then
# This builds the image to be flashed - it's normally done in targetenv_esp32s3_docker
   echo "Converting the elf file to binary image.... (This is usually done by make targetenv_esp32s3)"
   $python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.4/components/esptool_py/esptool/esptool.py --chip esp32s3 elf2image --min-rev-full 0 --max-rev-full 9999 -ff 80m -fm qio -fs 8MB -o $PATH_TO_BINARIES/ehs.bin $PATH_TO_BINARIES/ehs.exe
   $python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.4/components/esptool_py/esptool/esptool.py --chip esp32s3 merge_bin -o ehs.img --flash_mode dio --flash_size 8MB 0x0 ../ert-contrib-middleware/target_libs/xtensa-esp32s3_freertos-xtensa-esp32s3-elf-4.4.4/build/lib/bootloader.bin 0x9000 ../ert-contrib-middleware/target_libs/xtensa-esp32s3_freertos-xtensa-esp32s3-elf-4.4.4/build/lib/partition-table.bin 0x10000 $PATH_TO_BINARIES/ehs.bin
fi

#note the 0x0 before the binary name is the offset where to flash the image - this can be adapted to just update the ehs.bin at some toher location and leave the bootloader in place etc.
if [ "$1" = "-erase" ] ; then
	$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.4/components/esptool_py/esptool/esptool.py --chip esp32s3 --port /dev/ttyACM0 -b 460800 --before default_reset --after hard_reset erase_flash #-fm dio -fs 8MB -ff 80m 
fi
$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.4/components/esptool_py/esptool/esptool.py --chip esp32s3 --port /dev/ttyACM0 -b 460800 --before default_reset --after hard_reset write_flash -fm dio -fs 8MB -ff 80m 0x0 $PATH_TO_BINARIES/ehs.img

