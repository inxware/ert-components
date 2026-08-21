#!/bin/bash
set -x
echo "TEST_NAME=${TEST_NAME}"
echo "get the list of lpclink serials and ports from devman"
lines=`python3 ./SystemTests/CI/projects/heatrod/get-lpclink-info.py --group=${TEST_NAME}`
OLDIFS=$IFS
IFS=','
while read serial port
do
    echo "serial=${serial} port=${port}"
    if [ -z "${port}" ]; then
        echo "no port"
	else
        #do a double kill since the previous kill seems to always fail
        pids=`ps ax|grep arm-none-eabi-gdb-py|grep ${port}|awk '{ print $1 }'`
        while IFS= read -r line; do
            sudo kill -9 ${line}
        done <<< "${pids}"
        IFS=','
        sleep 5
        PORT=${port} SERIAL=${serial} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugServer.sh
        sleep 5
        ../ert-build-support/toolchains/x86_64/nxp-arm/bin/arm-none-eabi-gdb-py --init-eval-command="target remote localhost:${port}" -x ./SystemTests/CI/projects/heatrod/gdbClientFlashScript
        sleep 5
        PORT=${port} SERIAL=${serial} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugServer.sh
        sleep 5
        #start the logging again
        PORT=${port} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugClientLog.sh
    fi
done <<< ${lines}
IFS=$OLDIFS