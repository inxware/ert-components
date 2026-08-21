#!/system/bin/sh

# Waring! do not remove comment below
# ehs-android-update-script

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"

UPDATE_DATA_FOLDER=$1
DATA_ZIP="$UPDATE_DATA_FOLDER/dldata.tgz"

InstallUpdate(){
	APK=$1
	VALID=$( IsApkValid $APK )
	if [ -z "$VALID" ]; then
		InstallApp $APK
		echo "App ($APK) updates installed."
	else
		echo "Failed to install updates for $APK."
	fi
}

# check if the data zip is present
if [ -f "$DATA_ZIP" ]; then
	AMBIFIER_PACKAGE="com.TheSoundAgency.Ambifier2"
	EHS_PACKAGE="com.inx.ehs"
	DATA_FOLDER="$UPDATE_DATA_FOLDER/data"
	mkdir $DATA_FOLDER
	AMBIFIER_APK="$DATA_FOLDER/ambifier.apk"
	EHS_APK="$DATA_FOLDER/ehs.apk"
	Untar "$DATA_ZIP" "$DATA_FOLDER"
	InstallUpdate $AMBIFIER_APK
	sleep 1
	InstallUpdate $EHS_APK
	RestartApp
else
	echo "Data zip not found."
fi
