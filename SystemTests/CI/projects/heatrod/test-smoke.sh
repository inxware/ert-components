#!/bin/bash
set -x
group=${1}
echo "test-smoke.sh group=${group} TEST_PATH=${TEST_PATH} INX_NO_DELAY=${INX_NO_DELAY}"
cd ${TEST_PATH}
if [[ -z "${INX_NO_DELAY}" ]]; then
    #wait for the device to come back online
    sleep 60
fi
python3 test-smoke.py --group ${group}