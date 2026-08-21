#!/bin/bash
set -x
echo "tests.sh SERVER=${SERVER} TEST_PATH=${TEST_PATH}"
cd ${TEST_PATH}
python3 tests.py
