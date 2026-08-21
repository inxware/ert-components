#!/bin/bash
#must be root for the right usb permissions
#not sure what this does -swoport 2332
#not sure what this does -telnetport 2333
set -x
sudo nohup ../ert-build-support/debuggers/SEGGER/JLink_V646j/JLinkGDBServerCLExe -nosilent -select USB=${SERIAL} -singlerun -endian little -noir -speed auto -port ${PORT} -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction  > output.txt &

