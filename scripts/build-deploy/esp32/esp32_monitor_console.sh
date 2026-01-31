#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
if [ "${TOOLCHAIN_NAME}" = "" ]; then
TOOLCHAIN_NAME=xtensa-esp32s3-elf-4.4.1
fi

source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

rm -f out.txt
dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt" > out.txt
if [ "$1" == --use-screen ]; then
  TTYTERMINAL="screen -L -Logfile  out.txt"
  echo "Using screen. Use <CTRL>+d to exist and <CTRL>+<Escape> for scrollable TTY"
else
  TTYTERMINAL="minicom -D"
fi

if [ -c /dev/ttyUSB0 ]; then
ESP_TTY=/dev/ttyUSB0
elif [ -c /dev/ttyUSB1 ]; then
ESP_TTY=/dev/ttyUSB1
elif [ -c /dev/ttyACM0 ]; then
ESP_TTY=/dev/ttyACM0
elif [ -c /dev/ttyACM1 ]; then
ESP_TTY=/dev/ttyACM1
else
  echo "Could not find /dev/ttyUSB0 or /dev/ttyACM0"
  echo "If you are running in WSL, consider installing  usbipd"
  echo "https://github.com/dorssel/usbipd-win"
  echo "Hint: Use ADMIN POWERSHELL prompt to run something like this once installed:"
  echo "usbipd.exe list (find the JTAG entry)"
  echo "usbipd.exe bind --busid=2-2"
  echo "Then run the following from a WSL terminal"
  echo "usbipd.exe attach --wsl --busid=2-2"
  exit 1
fi

echo "If you need to use a terminal please try the web line editing enabled terminal here:"
echo "https://espressif.github.io/esptool-js/"
echo " OR Use  putty and enable \"local line editing\" in the config"
#  sudo screen -L -Logfile  out.txt $ESP_TTY 115200
#sudo $TTYTERMINAL $ESP_TTY
$TTYTERMINAL $ESP_TTY
