#!/bin/bash
set -e
#Build the currently configured target or the one specified as argument 1


if [ "$1" != "" ]; then
   ESP32_TARGET=$1
   ./configure ${ESP32S3_TARGET}
# Check if we are uploading for android platform
 if [ "$ESP32_TARGET" != "*esp32*" ]; then
    echo "**********************************************************************************************"
    echo "This script works only with esp32 tragets. esp32* target must be passed as first argument."
    echo "Run './configure' to see all targets."
    echo "**********************************************************************************************"
    exit 0
 fi

else 
  echo "Building "
  cat ./TARGET.cfg
  read -n 1 -p "PRESS y if this is what you want to build." ANSWER
  if [ "$ANSWER" != "y" ]; then
    echo "Aborting build"
    exit 1
  fi
fi
make clean ||:
make targetenv_prebuild
make targetenv_littlefs
make all_docker
make targetenv_esp32_docker


