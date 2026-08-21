#!/bin/bash
set -x
logpath="/home/jenkins/${PORT}.fifo"
if [ -f "${logpath}" ]; then
    echo ""
else
    mkfifo ${logpath}
fi
sudo /home/jenkins/startLoggingLpcLink.sh ${PORT}.fifo &
sudo nohup ../ert-build-support/toolchains/x86_64/nxp-arm/bin/arm-none-eabi-gdb-py -ex="set logging file ${logpath}" -ex="target remote localhost:${PORT}" -x ./SystemTests/CI/projects/heatrod/gdbClientLogScript > output.txt &