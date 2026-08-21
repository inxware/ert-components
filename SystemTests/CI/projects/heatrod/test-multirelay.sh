#!/bin/bash
set -x
group=${1}
if [ -z "${group}" ]; then
	echo "first argument must be the name of the group of devices you are testing"
	exit
fi
echo "test-multirelay.sh group=${group} TEST_PATH=${TEST_PATH}"
cd ${TEST_PATH}
python3 test-multirelay.py --group=${group}
