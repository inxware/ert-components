#!/bin/bash
#must be root for the right usb permissions

echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "Run this in another shell to see the debugger outputt (and reset the device)"
echo "./scripts/build-deploy/backer-hrdx/startlogging.sh "
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"


if [ -z "${PORT}" ]; then
       PORT=2331
fi
if [ -f "output.txt" ]; then
	rm output.txt
fi

../ert-build-support/toolchains/x86_64/arm-nxp/bin/arm-none-eabi-gdb-py -ex="target remote localhost:$PORT" -x ./scripts/build-deploy/heatrodCommissioning/runtime/gdbClientLogCLIScript

#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -select USB=$SERIAL -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0x>
#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0xxx12 -if SWD -halt -r>
#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -select USB=$SERIAL -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction

#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -singlerun -endian little -noir -speed auto -port 2331 -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction

#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent  -singlerun -endian little -noir -speed auto -port ${PORT} -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction 
#else
#echo "USING JTAG WITH SERIALNUMBER ${SERIAL}"
#../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -select USB=${SERIAL} -singlerun -endian little -noir -speed auto -port ${PORT} -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction 
#fi
