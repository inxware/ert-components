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

text="$*"

# Extract only the PC address (left side of each PC:SP pair, e.g. 0x420c193a:0x3fcc8ae0)
for pair in $(echo "$text" | tr ' ' '\n' | grep -E '^0x[0-9a-fA-F]+:0x[0-9a-fA-F]+$'); do
    pc="${pair%%:*}"
    ../ert-build-support/toolchains/x86_64/xtensa-esp32s3-elf-5.1/bin/xtensa-esp32s3-elf-addr2line -pfiaC -e ${EXE_FILE} "$pc"
done
