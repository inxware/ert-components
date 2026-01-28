#!/bin/bash

# Set the target...

if [ "$1" != "" ]; then
    echo "Setting TARGET to ${TARGET}"
    export TARGET=$1
else
source TARGET.cfg
fi
if [ -z "$TARGET" ]; then
    echo "TARGET is not set. Please set the TARGET environment variable or pass it as an argument."
    exit 1
fi

echo " Flash Arduino RP2040?"
echo "---------------------------------------------------------------------------------------------------------------------------"

TARGET_ROOT=../TARGET_TREES/ehs_env-${TARGET}
# uncomment for verbose option
#ERT_ARDUINO_VERBOSE=--verbose
ERT_ARDUINO_PROJECT=eRT_Debug
#ERT_ARDUINO_PROJECT=eRT_Release
ERT_ARDUINO_PROJECT_PATH=${TARGET_ROOT}/projects/${ERT_ARDUINO_PROJECT}

echo "Flashing ..."
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:mbed_nano:nanorp2040connect --verbose ${ERT_ARDUINO_PROJECT_PATH}

echo "---------------------------------------------------------------------------------------------------------------------------"
echo "All Done!"
