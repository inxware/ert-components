#!/bin/bash
if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000000
fi

port=2331
PORT=${port} SERIAL=${SERIAL} ./runjlinkserver.sh
sleep 5
./bin/arm-none-eabi-gdb-py -ex="target remote localhost:${port}" -x ./gdbClientLogCLIScript
