#!/bin/bash
firmwareFilePath="${1}"
if [ -z "${firmwareFilePath}" ]; then
#	echo "Please set first argument to the path to the ehs.exe you want to flash."
#	exit
    source ./TARGET.cfg
    firmwareFilePath="../../../../TARGET_TREES/ehs_env-${TARGET}/bin/ehs.exe"
fi

file ${firmwareFilePath} || exit 1

if [ -z "${SERIAL}" ]; then
	SERIAL=000611000000
fi

if [ -f  "${firmwareFilePath}" ]; then
	echo  "elf file ${firmwareFilePath} exists:"
	file  "${firmwareFilePath}" 
	port=2331
	cp ${firmwareFilePath} ./ehs.exe
	echo "JTAG SERIAL=${SERIAL}"
	PORT=${port} SERIAL=${SERIAL} ./runjlinkserver.sh || exit
	sleep 5
	gdbOutput=$(./bin/arm-none-eabi-gdb-py --init-eval-command="target remote localhost:${port}" -x ./gdbClientFlashScript -batch 2>&1)
	sleep 5
	if [[ ${gdbOutput} == *"Error in sourced command file"* ]]; then
		exit 1
	else
		exit 0
	fi
else
 	echo "ERROR the ELF file ${firmwareFilePath} does not exist"
fi
