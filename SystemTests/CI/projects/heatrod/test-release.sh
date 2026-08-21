#!/bin/bash
set -x
group=${1}
if [ -z "${group}" ]; then
	echo "first argument must be the name of the group of devices you are testing"
	exit
fi
echo "test-release.sh group=${group} TEST_PATH=${TEST_PATH} INX_NO_DELAY=${INX_NO_DELAY}"
cd ${TEST_PATH}
if [[ -z "${INX_NO_DELAY}" ]]; then
    #wait for the device to come back online
    sleep 60
fi
python3 test-release.py --group=${group}
