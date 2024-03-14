#!/system/bin/sh

# Warning! do not remove comment below
# ehs-android-update-script

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

UPDATE_DATA_FOLDER=$1
DATA_ZIP="$UPDATE_DATA_FOLDER/dldata.tgz"

InstallUpdate(){
	XML="$1/a64_paths.xml"
	if [ -f "$XML" ]; then
		mount -o rw,remount /system
		sleep 1
		SUCCESS=$( cp ${XML} "/system/etc/a64_paths.xml" )
		sleep 1
		mount -o ro,remount /system
		if [ -z "$SUCCESS" ]; then
			echo "Audio paths ($XML) patch installed."
		else
			echo "Audio paths ($XML) patch install FAILED. Error:($SUCCESS)"
		fi		
	else
		echo "Failed to install updates, audio paths file ($XML) is not present."
	fi
}

# check if the data zip is present
if [ -f "$DATA_ZIP" ]; then
	DATA_FOLDER="$UPDATE_DATA_FOLDER/data"
	mkdir $DATA_FOLDER
	Untar "$DATA_ZIP" "$DATA_FOLDER"
	InstallUpdate $DATA_FOLDER
	sleep 1
else
	echo "Data zip not found."
fi
