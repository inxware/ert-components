#!/bin/bash
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
#source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

# setup env for esp tools
#source ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/export.sh
source TARGET.cfg

TARGET_ENV_NAME="ehs_env-${TARGET}"
EXE_FILE=../TARGET_TREES/${TARGET_ENV_NAME}/bin/ehs.exe

text=$1

# Use awk to extract values before ':'
result=$(echo "$text" | awk -F ':' '{ for (i = 1; i < NF; i++) printf $i " "; }')

# Split the extracted values into an array
IFS=' ' read -ra values <<< "$result"

# Iterate through the extracted values
for value in "${values[@]}"; do
    if [ "$value" != "Backtrace:" ]; then
        ../ert-build-support/toolchains/x86_64/xtensa-esp32s3-elf-5.1/bin/xtensa-esp32s3-elf-addr2line -pfiaC -e ${EXE_FILE} "$value"
    fi
done
