#!/system/bin/sh

# Waring! do not remove comment below
# ehs-android-update-script

if [ 1 == 0 ] ; then
pushd ${1}
tar -xzf dldata.tgz
pm install -g -r player.apk
sleep 1

pm set-home-activity com.inx.ehs/.EhsHomeNativeActivity

popd
else
# above not yet working, but is the way we should be doing this (no dependency on the installed supervisor

#below doesn't work either. (Can't find installed scripts)

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

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
	EHS_PACKAGE="com.inx.ehs"
	DATA_FOLDER="$UPDATE_DATA_FOLDER/data"
	mkdir $DATA_FOLDER
	EHS_APK="$DATA_FOLDER/player.apk"
	Untar "$DATA_ZIP" "$DATA_FOLDER"
	InstallUpdate $EHS_APK
	sleep 1
	SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
else
	echo "Data zip not found."
fi
fi