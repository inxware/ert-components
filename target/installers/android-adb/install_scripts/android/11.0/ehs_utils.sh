#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils_common.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

StartService(){
	SERVICE=$1
	echo "Starting Service $SERVICE ..."
	am start-foreground-service $SERVICE
}

SetAsHomeApp(){
	PACKAGE=$1
	INTENT=$2
	if ! [ -z "$INTENT" ]; then
		echo "Setting ($PACKAGE) as home app ..."
		pm set-home-activity $PACKAGE/$INTENT
		LaunchHomeApp
	fi
}

GetHomeApp(){
	dumpsys activity | grep mHomeProcess
}

RestartNetwork(){
	SupervisorLog "RestartNetwork - not implemented for android 9.0"
}

IsApkValid(){
	APK=$1
	# skip validating
	# busybox doesn't seem to be available on rpi4 android11, is it the same on other tragets?
	#VALID=$(busybox unzip -l $APK | grep .xml)
	#if [ -z "$VALID" ]; then
	#	echo "invalid"
	#fi	
}

GetFocusedActivity(){
	dumpsys activity activities | grep -E 'mResumedActivity'
}

# this is needed for Android 11+, as the apps cannot read data from elsewhere,
# but only from the application scope 
CopyEhsDeviceID(){
#    SupervisorLog "INFO: Copying the device ID to com.inx.ehs file system"
    ID_STR=$1
    if [ -n "$ID_STR" ]; then
#    	SupervisorLog "INFO: NOT found $ID_STR - so going to create it.."
        ID_DST=/storage/emulated/0/Android/data/com.inx.ehs/files
		ID_DST_FILE=$ID_DST/.EHS/.ehs
		ID_FILE_STR=""
		if [ -f "$ID_DST_FILE" ]; then
			ID_FILE_STR="$(cat $ID_DST_FILE)"
		fi
		if [ "$ID_FILE_STR" != "$ID_STR" ]; then
			mkdir -p "$ID_DST/.EHS" ||:
			chmod a+rwx "$ID_DST/.EHS" || SupervisorLog "ERROR!! Couldn't chmod .EHS"
			if [ -d "$ID_DST/.EHS" ]; then
				echo "$ID_STR" > "$ID_DST/.EHS/.ehs" ||:
				chmod a+rw "$ID_DST/.EHS/.ehs" || SupervisorLog "ERROR!! Couldn't chmod .EHS/.ehs"
	#	    Android doesn't prioritise flushingthis data to disk - seems to miss attribute changes altogether...
			sync
	#	    SupervisorLog $( ls -l "/storage/emulated/0/Android/data/com.inx.ehs/files/.EHS/.ehs" )
			else
				SupervisorLog "eRT storage ($ID_DST) is not availbe. Has eRT been installed?"
			fi
		else
			SupervisorLog "eRT app storage id ($ID_STR) is up-to-date"
		fi
    else
       SupervisorLog "ERROR!!!: ID_STR is not set can't store the device ID"
    fi
#    SupervisorLog $( ls -l "/storage/emulated/0/Android/data/com.inx.ehs/files/.EHS/.ehs" )
}
