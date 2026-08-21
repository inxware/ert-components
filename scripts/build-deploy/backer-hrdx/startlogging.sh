#!/bin/bash
if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000002
fi

if [ 1 = 0 ];then
port=2331
PORT=${port} SERIAL=${SERIAL} ./scripts/build-deploy/backer-hrdx/runjlinkserver.sh
sleep 5
fi
# This seems to be nonsense:
#../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py -ex="target remote localhost:${port}" -x ./scripts/build-deploy/backer-hrdx/gdbClientLogCLIScript

echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "Run this in another shell to see the debugger outputt:"
echo '../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py -ex="target remote localhost:2331" -x ./scripts/build-deploy/heatrodCommissioning/runtime/gdbClientLogCLIScript'
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -select USB=$SERIAL -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction
../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction

