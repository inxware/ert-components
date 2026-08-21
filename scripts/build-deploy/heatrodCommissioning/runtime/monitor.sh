#!/bin/bash
date=`date +"%Y-%m-%dT%T"`
echo "${date} ${1}"
if [[ ${1} == *"monitorok"* ]]; then
	touch ./monitor.flag
elif [[ ${1} == *"devicepin_"* ]]; then
	pin=$(echo "${1}"| cut -d'_' -f 2)
	echo "${pin}" > ./pin.txt
elif [[ ${1} == *"cpuid_"* ]]; then
	serial=$(echo "${1}"| cut -d'_' -f 2)
	echo "${serial}" > ./serial.txt
elif [[ ${1} == *"Program received signal SIGTRAP"* ]]; then
	touch ./powoff.flag
fi
