#!/system/bin/sh

# Waring! do not remove comment below
# ehs-android-update-script

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

UPDATE_DATA_FOLDER=$1
DATA_ZIP="$UPDATE_DATA_FOLDER/dldata.tgz"

InstallApkUpdates(){
	APK=$1
	VALID=$( IsApkValid $APK )
	if [ -z "$VALID" ]; then
		InstallApp $APK
		sleep 3
		echo "App ($APK) updates installed."
	else
		echo "Failed to install updates for $APK."
	fi
}

InstallSupervisorUpdates(){
    SUPERVISOR_DATA=$1
	if [ -d "$SUPERVISOR_DATA" ]; then 
		mount -o rw,remount /
		sleep 2
	    MSG=$( cp -rf ${SUPERVISOR_DATA} / )
	    sleep 2
		mount -o ro,remount /
		sleep 2
	    echo "$MSG"
	else
		echo "Failed to update supervisor. Supervisor data unavilable."
	fi
}

# check if the data zip is present
if [ -f "$DATA_ZIP" ]; then
	DATA_FOLDER="$UPDATE_DATA_FOLDER/data"
	mkdir $DATA_FOLDER
	SUPERVISOR_SCRIPTS="$DATA_FOLDER/system"
	DOWNLOADER_APK="$DATA_FOLDER/downloader.apk"
	Untar "$DATA_ZIP" "$DATA_FOLDER"
	# install supervisor
	MSG1=$( InstallSupervisorUpdates $SUPERVISOR_SCRIPTS )
    # install downloader
	MSG2=$( InstallApkUpdates $DOWNLOADER_APK )
    echo "$MSG1, $MSG2"
	RebootDevice
else
	echo "Data zip not found."
fi
