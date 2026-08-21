#!/bin/bash
set -x
echo "test-ota.sh TEST_PATH=${TEST_PATH} ROUTER_HOST=${INX_TEST_ROUTER_HOST}"

if [ -z "${INX_TEST_ROUTER_HOST}" ]; then
    echo ""
else
    INX_TEST_ROUTER_HOST="--routerHost ${INX_TEST_ROUTER_HOST}"
fi

cd ${TEST_PATH}
python3 test-ota.py ${INX_TEST_ROUTER_HOST}