#!/bin/bash
#set -x
firmwareFilePath="${1}"
if [ -z "${firmwareFilePath}" ]; then
    source ./TARGET.cfg
    firmwareFilePath="../TARGET_TREES/ehs_env-${TARGET}/bin/ehs.exe"
fi

if [ -z "${SERIAL}" ]; then
	SERIAL=000611000002
fi

if [ -f  "${firmwareFilePath}" ]; then
  echo  "elf file ${firmwareFilePath} exists:"
  file  "${firmwareFilePath}" 
  cp ${firmwareFilePath} ./ehs.exe
  port=2331
  cp ${firmwareFilePath} ./
  echo "serial=${SERIAL} port=${port}"
#todo the following script is probably now in  ./scripts/hrd../ and proably doesn't have the irretating need toknow rhe serial number of the device.
  sudo PORT=${port} SERIAL=${SERIAL} ./SystemTests/CI/projects/heatrod/jenkinsRunJlinkDebugServer.sh
  sleep 5
  ../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py --init-eval-command="target remote localhost:${port}" -x ./SystemTests/CI/projects/heatrod/gdbClientFlashScript
  sleep 5
  cat output.txt
else 
  echo "ERROR the ELF file ${firmwareFilePath} does not exist"
fi

#./scripts/build-deploy/backer-hrdx/startlogging.sh
