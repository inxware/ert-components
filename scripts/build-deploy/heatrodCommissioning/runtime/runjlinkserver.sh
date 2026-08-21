#!/bin/bash
#must be root for the right usb permissions
#not sure what this does -swoport 2332
#not sure what this does -telnetport 2333
if [ -z "${SERIAL}" ]; then
	echo "Please set the environment variable SERIAL to the serial number of your debugger."
	exit
fi

if [ -z "${PORT}" ]; then
	echo "Please set the environment variable PORT to the local port you want the jlink server to listen on."
	exit
fi
if [ -f "output.txt" ]; then
	rm output.txt
fi
if [ "${SERIAL}"="ANYJTAG" ]; then
echo "USING DEFAULT JTAG DEVICE"
nohup ./JLinkGDBServerCLExe -nosilent  -singlerun -endian little -noir -speed auto -port ${PORT} -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction > output.txt 2>&1 &
else
echo "USING JTAG WITH SERIALNUMBER ${SERIAL}"
nohup ./JLinkGDBServerCLExe -nosilent -select USB=${SERIAL} -singlerun -endian little -noir -speed auto -port ${PORT} -vd -device MK64FN1M0xxx12 -if SWD -halt -reportuseraction > output.txt 2>&1 &
fi
