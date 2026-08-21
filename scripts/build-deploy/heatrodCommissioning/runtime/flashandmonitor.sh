#!/bin/bash
if [[ -z "${SERIAL}" ]]; then
	SERIAL=000611000001
fi

if [ "root" != "${USER}" ]; then
	echo "You must be root to use these scripts."
	exit
fi
variant=""
doNextBoard=1
while [ ${doNextBoard} -gt 0 ]; do
	if [ -z "${variant}" ]; then
		#display menu for choosing variant
		options=(`ls ../firmwares`)
		echo "Please choose the board variant."
		select opt in "${options[@]}"; do
			variant="${opt}"
			break
		done
	fi
	select opt in "Start testing" "Flash production NO TESTING" "Change board variant" "Quit"; do
		case $opt in
			"Start testing")
				break
				;;
			"Flash production NO TESTING")
				docommission="no"
				doProductionFlash=1
				echo "PRODUCTION_FLASH $variant" > ./serial.txt
				echo "NO PIN" > ./pin.txt
				break
				;;
			"Change board variant")
				doProductionFlash=0
				variant=""
				break
				;;
			"Quit")
				echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				variant=""
				doNextBoard=0
				doProductionFlash=1
				break
				;;
			*)
				echo "Invalid option $REPLY"
				;;
		esac
	done
	if [ -z "${variant}" ]; then
		echo "Quitting..."
	else
	     if [ -z "${docommission}" ]; then
		echo "Flashing commissioning firmware to device..."
		SERIAL=${SERIAL} ./flash.sh ../firmwares/${variant}/commission.bin
		exitCode=$?
		if [ ${exitCode} -eq 0 ]; then
			echo "Flashing succeeded. Waiting for results..."
			monitorFlagPath="./monitor.flag"
			powoffFlagPath="./powoff.flag"
			if [ -f "${monitorFlagPath}" ]; then
				rm ${monitorFlagPath}
			fi
			if [ -f "${powoffFlagPath}" ]; then
				rm ${powoffFlagPath}
			fi
			if [ -f "./serial.txt" ]; then
				rm ./serial.txt
			fi
			if [ -f "./pin.txt" ]; then
				rm ./pin.txt
			fi
			SERIAL=${SERIAL} ./startlogging.sh | sed -u 's/ /\\ /g' | xargs -n1 ./monitor.sh &
			#now we want to loop round waiting for a signal from monitor.sh
			waitForMonitor=120
			doProductionFlash=0
			while [ ${waitForMonitor} -gt 0 ]; do
					sleep 1
					waitForMonitor=$((waitForMonitor-1))
					if [ -f "${monitorFlagPath}" ]; then
							waitForMonitor=0
							doProductionFlash=1
					fi
					if [ -f "${powoffFlagPath}" ]; then
							waitForMonitor=0
							doProductionFlash=0
					fi
					echo -n "."$waitForMonitor
			done
			killall arm-none-eabi-gdb-py
			killall xargs
			sleep 5
			echo "PASSED.."
		else
			echo "Failed to flash the device. Please check your JTAG cable and board power."

		fi # exit code was not zero
	   fi # end of skip testing ...
		if [ ${doProductionFlash} -gt 0 ]; then
			echo "Now flashing release firmware..."
			SERIAL=${SERIAL} ./flash.sh ../firmwares/${variant}/release.bin
			exitCode=$?
			echo "EXITED WITH ${exitCode}"
			if [ ${exitCode} -eq 0 ]; then
				echo "PASSED. Printing label..."
				serial=`cat serial.txt`
				pin=`cat pin.txt`
				INX_SERIAL=${serial} INX_PIN=${pin} ./printlabel.sh
				echo "LABEL #1 printing..."
				echo "Press any key for label #2"
				read -n 1 
				INX_SERIAL=${serial} INX_PIN=${pin} ./printlabel.sh
			else
				echo "Failed to flash the release firmware. Please check your JTAG cable and board power."
			fi
		fi
	fi ## quit if/ else
done
