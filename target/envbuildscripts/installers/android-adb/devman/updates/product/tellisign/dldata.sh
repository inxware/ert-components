#!/system/bin/sh

# Waring! do not remove comment below
# ehs-android-update-script

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"

UPDATE_DATA_FOLDER=$1
DATA_ZIP="$UPDATE_DATA_FOLDER/dldata.tgz"
# check if the data zip is present
if [ -f "$DATA_ZIP" ]; then
	PACKAGE="online.show.signplayer"
	HOME_INTENT="com.unity3d.player.UnityPlayerHomeActivity"
	DATA_FOLDER="$UPDATE_DATA_FOLDER/data"
	mkdir $DATA_FOLDER
	SSO_APK="$DATA_FOLDER/tellisign.apk"
	Untar "$DATA_ZIP" "$DATA_FOLDER"
	VALID=$( IsApkValid $SSO_APK )
	if [ -z "$VALID" ]; then
		InstallApp $SSO_APK
		RestartApp
	else
		echo "Failed to install SSO updates."
	fi
else
	echo "Data zip not found."
fi
