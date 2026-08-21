#!/bin/bash
set -e

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

echo "HAVE YOU RUN make targetenv_esp32 yet???????"

python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py -p /dev/ttyUSB0 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash -z --flash_mode "dio" --flash_size detect --flash_freq 40m 0x1000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/bootloader.bin 0x8000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/partitions.bin 0x10000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/ehs.bin
#$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash -z --flash_mode "dio" --flash_size detect --flash_freq 40m 0x1000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/bootloader.bin 0x8000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/partitions_singleapp.bin 0x10000 ehs.bin

