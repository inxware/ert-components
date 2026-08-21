#!/bin/bash
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
source "${SCRIPT_DIR}/../../../target/envbuildscripts/targetenv_esp32_sdk-packager_paths.sh"
# TOOLCHAIN_FLASHING_VERSION
# CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION

echo "HAVE YOU RUN make targetenv_esp32 yet???????"

python ../ert-contrib-middleware/contrib/esp-idf/${CONTRIB_MIDDLWARE_FLASHINGTOOLS_VERSION}/components/esptool_py/esptool/esptool.py erase_fash
