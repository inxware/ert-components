#!/bin/bash

TEMP_DIR="/sdcard/.ehs_temp"

adbconnect(){
	IP=$1
	if [ -z "$IP" ]; then
		$ADB root
	else
		echo "Connecting adb to $IP"
		$ADB connect "$IP"
		sleep 1
		$ADB root
		sleep 1
		$ADB connect "$IP"
	fi
	sleep 2
	$ADB remount || exit 1
}

macgen(){
	FIRST_6="$1"
	# create a randoom number
	SEED=$(uuidgen)
	# create mac address
	if [ -z "$FIRST_6" ]; then
		echo $SEED|md5sum|sed 's/^\(..\)\(..\)\(..\)\(..\)\(..\).*$/02:\1:\2:\3:\4:\5/'
	else
		NEW_MAC=$(echo $SEED|md5sum|sed 's/^\(..\)\(..\)\(..\)\(..\)\(..\).*$/02:\1:\2:\3:\4:\5/')
		echo $NEW_MAC|sed -r "s/^(.{0})(.{8})/\1$FIRST_6/"
	fi
}

macread(){
	echo "TEST_ID_A6"
}

mktemp(){
	$ADB shell 'mkdir '${TEMP_DIR} || exit 1
}

rmtemp(){
	$ADB shell 'rm -r '${TEMP_DIR} || exit 1
}

cptemp(){
	SRC=$1
	$ADB push ${SRC} ${TEMP_DIR} || exit 1
}

temp(){
	echo "$TEMP_DIR"
}

install(){
	APK=$1
	echo "Installing $APK ..."
	$ADB shell 'pm install -g -r '${TEMP_DIR}/${APK} || exit 1
}

uninstall(){
	PACKAGE=$1
	INSTALLED=$( $ADB shell 'pm list packages | grep '${PACKAGE} )
	if ! [ -z "$INSTALLED" ]; then
		echo "Uninstalling $PACKAGE ..."
		$ADB shell 'pm uninstall '${PACKAGE} || exit 1
	fi
}

starthomeapp(){
	PACKAGE=$1
	INTENT=$2
	$ADB shell 'pm set-home-activity '${PACKAGE}'/'${INTENT} || exit 1
	sleep 1
	$ADB shell 'am start -a android.intent.action.MAIN -c android.intent.category.HOME' || exit 1
}

startdownloader(){
	$ADB shell 'am start-foreground-service com.utils.downloader/.DownloadService' || exit 1
}

downloader_cert(){
	ADDRESS=$1
	CERT_TYPE=$2
	CERT_PATH=$3
	$ADB shell "am broadcast -a com.utils.downloader.broadcastreceiver.CERTIFICATE --es 'ip_address' '$ADDRESS' --es '$CERT_TYPE' '$CERT_PATH'" || exit 1
}

list_files(){
	ADB_DIR=$1
	$ADB shell "ls $ADB_DIR"
}

count_files(){
	ADB_DIR=$1
	$ADB shell "ls $ADB_DIR | wc -l"
}

check_android_version(){
	echo "=========== Checking android version ============="
	VERSION=$( $ADB shell 'getprop ro.build.version.release' )
	if [ "$VERSION" == "9" ]; then
		echo "Target device android version ($VERSION) matches."
	else
		echo "Target device android version ($VERSION) doesn't match configuration (9)."
		exit 1
	fi
}

install_certs(){
	echo "=========== Certificates Setup ============="
	chmod +x ./install_cert.sh || exit 1
	./install_cert.sh || exit 1
}

shell_exec(){
	ARGS=$1
	$ADB shell ${ARGS} || exit 1
}

shell_push(){
	SRC=$1
	DST=$2
	$ADB push ${SRC} ${DST} || exit 1
}

shell_push_ex(){
	shell_push $1 $2 || exit 1
}

write_server_config(){
	NAME=$1
	URL=$2
	shell_exec 'echo "export DEVMAN_SERVER_NAME=\"'${NAME}'\"" >> /system/etc/ehs_supervisor/ehs_server_configure.sh' || exit 1
	shell_exec 'echo "export SERVER_ADDRESS=\"'${URL}'\"" >> /system/etc/ehs_supervisor/ehs_server_configure.sh' || exit 1
}
