#!/bin/bash
set -e



echo "HAVE YOU RUN make targetenv_esp32 yet???????"

$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py erase_fash
#$python ../ert-contrib-middleware/contrib/esp-idf/esp-idf-4.4.1/components/esptool_py/esptool/esptool.py -p /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash -z --flash_mode "dio" --flash_size detect --flash_freq 40m 0x1000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/bootloader.bin 0x8000 ../TARGET_TREES/ehs_env-esp32_freertos-xtensor-base/bin/partitions_singleapp.bin 0x10000 ehs.bin
