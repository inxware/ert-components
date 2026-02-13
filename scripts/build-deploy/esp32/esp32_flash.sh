#!/bin/bash
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

source TARGET.cfg
export TEMP_PWD=${PWD}
export TOOLCHAIN_PATH=$TEMP_PWD/../ert-build-support/toolchains/x86_64/
_PATH="${TOOLCHAIN_PATH}/bin"
#export IDF_PYTHON_ENV_PATH="${TOOLCHAIN_PATH}/espressif-4.4.4/python_env/venv/bin"
#This is for docker environments: (Do we want this)?
export IDF_PYTHON_ENV_PATH="/opt/python_env/bin"

if [ ! -d ${IDF_PYTHON_ENV_PATH} ]; then
  echo "You do not have the python virtual environmwnt with the esptools installed in $IDF_PYTHON_ENV_PATH."
  echo "Please install using ..." 
 exit 1  
fi

#TODO 2025 - This all looks like a duplication of esptool across random directories?

_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/openocd-esp32/v0.11.0-esp32-20221026/openocd-esp32/bin"
_PATH="${_PATH:+${_PATH}:}${IDF_PYTHON_ENV_PATH}"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/esptool_py/esptool"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/espcoredump"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/partition_table"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/app_update"
_PATH="${_PATH:+${_PATH}:}${TOOLCHAIN_PATH}/${TOOLCHAIN_FLASHING_VERSION}/tools/additional_tools"
export PATH="${_PATH}${PATH:+:${PATH}}"

PATH_TO_BINARIES=../TARGET_TREES/ehs_env-$TARGET/bin/

#WARNING DONT ENABLE THIS AS IT BUILDS SPECIFIC ERT-CONTRIB PATHS..
if [ 0 = 1 ]; then
# This builds the image to be flashed - it's normally done in targetenv_esp32s3_docker, so disabling here for now.
   echo "Converting the elf file to binary image.... (This is usually done by make targetenv_esp32s3)"
   python3.10 ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py --chip ${ESP32_CHIP_VERSION} elf2image --min-rev-full 0 --max-rev-full 9999 -ff 80m -fm qio -fs 8MB -o $PATH_TO_BINARIES/ehs.bin $PATH_TO_BINARIES/ehs.exe
   python3.10 ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py --chip ${ESP32_CHIP_VERSION} merge_bin -o ehs.img --flash_mode dio --flash_size 8MB 0x0 ../ert-contrib-middleware/target_libs/xtensa-esp32s3_freertos-xtensa-esp32s3-elf-4.4.4/build/lib/bootloader.bin 0x9000 ../ert-contrib-middleware/target_libs/xtensa-esp32s3_freertos-xtensa-esp32s3-elf-4.4.4/build/lib/partition-table.bin 0x10000 $PATH_TO_BINARIES/ehs.bin
fi

if [ -z $DEVICE_TTY_PATH ]; then
if [ -c /dev/ttyUSB0 ]; then
  DEVICE_TTY_PATH="/dev/ttyUSB0"
elif [ -c /dev/ttyUSB1 ]; then
  DEVICE_TTY_PATH="/dev/ttyUSB1"
elif [ -c /dev/ttyACM0 ]; then
  DEVICE_TTY_PATH="/dev/ttyACM0"
elif [ -c /dev/ttyACM1 ]; then
  DEVICE_TTY_PATH="/dev/ttyACM1"
else
  echo "Could not find /dev/ttyUSB0 or /dev/ttyACM0"
  echo "If you are running in WSL, consider installing  usbipd"
  echo "https://github.com/dorssel/usbipd-win"
  echo "Hint: User Powershell to run something like this once installed:"
  echo "usbipd list"
  echo "usbipd bind --busid=2-2"
  echo "usbipd attach --wsl --busid=2-2"
  exit 1
fi
else
  if ! [ -c $DEVICE_TTY_PATH ]; then
    echo "Could not find $DEVICE_TTY_PATH"
    echo "If you are running in WSL, consider installing  usbipd"
    echo "https://github.com/dorssel/usbipd-win"
    echo "Hint: User Powershell to run something like this once installed:"
    echo "usbipd list"
    echo "usbipd bind --busid=2-2"
    echo "usbipd attach --wsl --busid=2-2"
    exit 1
  fi
fi

#note the 0x0 before the binary name is the offset where to flash the image - this can be adapted to just update the ehs.bin at some toher location and leave the bootloader in place etc.
if [ "$1" = "-erase" ] ; then
	python ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py --chip esp32s3 --port $DEVICE_TTY_PATH -b 460800 --before default_reset --after hard_reset erase_flash #-fm dio -fs 8MB -ff 80m 
fi
# And finally ectally falsh the board...
#Not sure if these lines different for a good reason, but to get rid of duplicated files - allowing for legacy difference here.

if [ "${ESP32_CHIP_VERSION}" = "esp32s3" ]; then
  echo "Flashing ESP32-S3"
   # For ESP32S£s we combine the images into a single image file (ehs.img)
   python ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py --chip ${ESP32_CHIP_VERSION} --port ${DEVICE_TTY_PATH} -b 460800 --before default_reset --after hard_reset write_flash -fm dio -fs 8MB -ff 80m 0x0 $PATH_TO_BINARIES/ehs.img
else
#esp32 still used seperate partition and boot.bin files
   python ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py -p ${DEVICE_TTY_PATH} -b 460800 --before default_reset --after hard_reset --chip ${ESP32_CHIP_VERSION}  write_flash -z --flash_mode "dio" --flash_size detect --flash_freq 40m 0x1000 ${PATH_TO_BINARIES}/bootloader.bin 0x8000 ${PATH_TO_BINARIES}/partition-table.bin 0x10000 ${PATH_TO_BINARIES}/ehs.bin 0x2ff000 ${PATH_TO_BINARIES}/app_data.bin
  echo "Flashing ESP32 (not S3)"
fi
# From the esp32 (not S3) versions
# python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash -z --flash_mode "dio" --flash_size detect --flash_freq 40m 0x1000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/bootloader.bin 0x8000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/partitions_singleapp.bin 0x10000 ehs.bin
