#!/bin/bash
firmwareFilePath="${1}"
if [[ -z "${firmwareFilePath}" ]]; then
	echo "Please set first argument to the path to the ehs.exe you want to flash."
	exit
fi

if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000000
fi

if [ "root" != "${USER}" ]; then
	echo "You must be root to use these scripts."
	exit
fi

SERIAL=${SERIAL} ./flash.sh ${firmwareFilePath}
SERIAL=${SERIAL} ./startlogging.sh
