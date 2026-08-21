#!/bin/bash
set -x
echo "test-webui.sh TEST_PATH=${TEST_PATH}"
cd ${TEST_PATH}
python3 test-webui.py