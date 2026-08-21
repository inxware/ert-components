#!/bin/bash
set -x
if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000002
fi

port=2331
sudo PORT=${port} SERIAL=${SERIAL} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugServer.sh
sleep 5
sudo ../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py -ex="target remote localhost:${port}" -x ./SystemTests/CI/projects/heatrod/gdbClientLogCLIScript
