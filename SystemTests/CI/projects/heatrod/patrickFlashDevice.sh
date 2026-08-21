#!/bin/bash
set -x
firmwareFilePath="${1}"
if [[ -z "${firmwareFilePath}" ]]; then
    firmwareFilePath="../TARGET_TREES/ehs_env-nxp_arm_patrick_hri_debug/bin/ehs.exe"
fi

if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000002
fi

port=2331
cp ${firmwareFilePath} ./
echo "serial=${SERIAL} port=${port}"
sudo PORT=${port} SERIAL=${SERIAL} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugServer.sh
sleep 5
../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py --init-eval-command="target remote localhost:${port}" -x ./SystemTests/CI/projects/heatrod/gdbClientFlashScript
sleep 5
