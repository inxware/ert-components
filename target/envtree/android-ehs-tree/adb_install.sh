#!/bin/bash
PLATFORM=""
PRODUCT=""
SERVER_NAME=""
ANDROID_VERSION=""
INSTALL_APK_DIR=""
REBOOT="FALSE"
ADB_IP=""
PACK_DIR=""

#todo2022 this file needs tto be separated into its part that makes an apk and its parth that installs iit on a device.
#todo2022 the part that installs it shoud be moved to the ./target/envtree/android-ehs-tree/install_scripts/  directory

if [ -z $ADB ]; then
	export ADB=adb
fi

if [ -z $EHS_ROOT ]; then
pushd "../../../"
export EHS_ROOT=$( pwd )
popd
fi

if [ -z $INSTALL_DIR ]; then
	export INSTALL_DIR=$( pwd )
fi
echo "INSTALLER DIR ==> $INSTALL_DIR"
echo "EHS ROOT DIR ==> $EHS_ROOT"

function help() {
    echo "Help for adb_install.sh"
	echo "  -av,    Android version."
	echo "  -avl,   List of android versions."
	echo "  -ad,    APKs directory."
	echo "  -h,     Show help."
	echo "  -ip,    ADB ip address."
	echo "  -pl,    Platform name."
	echo "  -pll,   List of all platforms."
    echo "  -pr,    Product name."
	echo "  -prl,   List of all products."
	echo "  -pck,   Specify directory where to pack supervisor."
	echo "  -r,     Reboot after installation."
	echo "  -sn,    Specify target server. Note: This must be specified for 'player' products."
    echo "  -snl,   List available server names."
}

function avl() {
	ls ${INSTALL_DIR}/install_scripts/android
}

function prl() {
	ls ${INSTALL_DIR}/install_scripts/product
}

function pll() {
	ls ${INSTALL_DIR}/install_scripts/platform
}

function snl() {
	ls ${INSTALL_DIR}/install_scripts/server
}

i=1
while [ "$i" -le "$#" ]; do
    eval "arg=\${$i}"
    if [[ $arg == "-h" ]]; then
        help
        exit 0
	elif [[ $arg == "-av" ]]; then
        i=$((i + 1))
        eval "android_version=\${$i}"
        export ANDROID_VERSION=$android_version
	elif [[ $arg == "-ad" ]]; then
        i=$((i + 1))
        eval "apk_dir=\${$i}"
        export INSTALL_APK_DIR=$apk_dir
    elif [[ $arg == "-avl" ]]; then
		# android version list
        avl
		exit 0
	elif [[ $arg == "-ip" ]]; then
        i=$((i + 1))
        eval "ip=\${$i}"
        ADB_IP=$ip
	elif [[ $arg == "-pl" ]]; then
        i=$((i + 1))
        eval "platform=\${$i}"
        PLATFORM=$platform
    elif [[ $arg == "-pr" ]]; then
        i=$((i + 1))
        eval "product=\${$i}"
        PRODUCT=$product
	elif [[ $arg == "-pll" ]]; then
		# platform list
        pll
		exit 0
    elif [[ $arg == "-prl" ]]; then
		# product list
        prl
		exit 0
	elif [[ $arg == "-r" ]]; then
		REBOOT="TRUE"
	elif [[ $arg == "-sn" ]]; then
        i=$((i + 1))
        eval "server=\${$i}"
        SERVER_NAME=$server
    elif [[ $arg == "-snl" ]]; then
        snl
		exit 0
	elif [[ $arg == "-pck" ]]; then
        i=$((i + 1))
        eval "pack_dir=\${$i}"
        export PACK_DIR=$pack_dir
    else
        echo "Invalid argument ($arg). Use -h for help."
        exit
    fi
    i=$((i + 1))
done

INSTALL_PRODUCT_DIR=${INSTALL_DIR}/install_scripts/product/${PRODUCT}
SET_UP_PLATFORM_DIR=${INSTALL_DIR}/install_scripts/platform/${PLATFORM}
export ANDROID_VERSION_DIR=${INSTALL_DIR}/install_scripts/android/${ANDROID_VERSION}

FAILED=false
if [ -z $PRODUCT ]; then
	echo "Product name (e.g -pr ehs) must be specified. Use '-prl' to list all products."
	FAILED=true
fi
if [ -z $PLATFORM ]; then
	echo "Platform name (e.g -pl pine64_h6) must be specified. Use '-pll' to list all platforms."
	FAILED=true
fi
if [ -z $ANDROID_VERSION ]; then
	echo "Android version (e.g -av 9.0) must be specified. Use '-avl' to list all versions."
	FAILED=true
fi
if ! [ -d $INSTALL_PRODUCT_DIR ]; then
	echo "Product directory ($INSTALL_PRODUCT_DIR) doesn't exist. $PRODUCT is not a valid product."
	FAILED=true
fi
if ! [ -d $SET_UP_PLATFORM_DIR ]; then
	echo "Platform directory ($SET_UP_PLATFORM_DIR) doesn't exist. $PLATFORM is not a valid platform."
	FAILED=true
fi
if ! [ -d $ANDROID_VERSION_DIR ]; then
	echo "Android version directory ($ANDROID_VERSION_DIR) doesn't exist. $ANDROID_VERSION is not a supported android version."
	FAILED=true
fi

if [ $FAILED == true ]; then
	exit 1
fi

# set-up server dir	
if [ -n "$SERVER_NAME" ]; then
	INSTALL_SERVER_DIR=${INSTALL_DIR}/install_scripts/server/${SERVER_NAME}
	if ! [ -d "$INSTALL_SERVER_DIR" ]; then
		echo "Unknown target server ($SERVER_NAME)"
		exit 1
	fi
else
	# set server based on product name
	INSTALL_SERVER_DIR=${INSTALL_DIR}/install_scripts/server/${PRODUCT}
	if ! [ -d "$INSTALL_SERVER_DIR" ]; then
		INSTALL_SERVER_DIR=""
	fi
fi

# make sure all files are of unix type
find $INSTALL_DIR -path $INSTALL_DIR/utils -prune -false -o -type f -print0 | xargs -0 dos2unix --

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1
source $SET_UP_PLATFORM_DIR/install_utils.sh || exit 1

if [ -n "$PACK_DIR" ]; then
	# this is uesd for creating an update package for the supervisor
	package_supervisor "$PACK_DIR"
	exit 0
fi

echo "Setting up adb ($ADB)"
# the following will rempount the system directories read/write mode
adbconnect $ADB_IP

# make sure the configuration android version matches the device 
check_android_version
# make sure the configuration android platform matches the device
check_platform

# upload supervisor
ROOT_DIR=${INSTALL_DIR}/root-dir
#shell_exec '/etc/ehs_supervisor/ehs_kill.sh'
$ADB shell 'rm /system/bin/ehs_*; rm /system/etc/ehs_supervisor/ehs_*;'
pushd $ROOT_DIR || exit 1
shell_push system /
popd
shell_exec 'chown root:shell /system/bin/ehs_* /system/etc/ehs_supervisor/*; chmod 0755 /system/bin/ehs_* /system/etc/ehs_supervisor/*;' || exit 1

# install downloader
pushd $INSTALL_DIR || exit 1
source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1
DOWNLOADER_APK=downloader.apk
DOWNLOADER_PACKAGE=com.utils.downloader
mktemp
cptemp utils/$DOWNLOADER_APK
uninstall ${DOWNLOADER_PACKAGE}
sleep 2
install ${DOWNLOADER_APK}
starthomeapp ${DOWNLOADER_PACKAGE} '.InitActivity'
sleep 2
rmtemp
popd

install_platform || exit 1

# clear previous data folders
EHS_DATA="/sdcard/.EHS"
EXISTS=$( shell_exec 'if [ -d "'${EHS_DATA}'" ]; then echo "YES"; fi' )
if ! [ -z "$EXISTS" ]; then
	echo "Clearing $EHS_DATA ..."
	shell_exec 'rm -r '${EHS_DATA}
fi

# add install apk(s) to the device
if [ -d "$INSTALL_APK_DIR" ]; then
	echo "Adding APKs to the install directories"
	INSTALL_APK_ANDROID="/sdcard/.EHS_INSTALL"
	shell_exec 'if [ -d "'${INSTALL_APK_ANDROID}'" ]; then rm -r "'${INSTALL_APK_ANDROID}'"; fi' || exit 1
	shell_exec 'mkdir -p "'${INSTALL_APK_ANDROID}'"' || exit 1
	shell_push ${INSTALL_APK_DIR}'/*' ${INSTALL_APK_ANDROID}'/' || exit 1
fi

install_product || exit 1
install_android || exit 1
install_server || exit 1

if [ "$REBOOT" = "TRUE" ]; then
	shell_exec 'reboot' || exit 1
fi
