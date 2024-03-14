#!/system/bin/sh

# this is depricated !! should be using "player" now


# this is called from the ehs supervisor
source "$EHS_SUPERVISOR_LOCATION/ehs_app_utils.sh"

EHS_PACKAGE="com.inx.ehs"

InitAppManger(){
	SupervisorLog "Initialising EHS."
	# Check if the package is installed
	INSTALLED=$( IsPackageInstalled $EHS_PACKAGE )
	if [ -z "$INSTALLED" ]; then
	#todo this should be an argument so we are not duplicating code al the time
		APK_NAME="ehs.apk"
		APK_LOCATION="$EHS_UPDATES_LOCATION/$APK_NAME"
		#DownloadApk $APK_NAME $APK_LOCATION $DOWNLOAD_ADDRESS "/software-download/$APK_NAME"
		InstallNewApk $APK_NAME $APK_LOCATION $EHS_PACKAGE
		StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
		RebootDevice
	else
		StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"	
	fi
	
}

RunAppManger(){
	# check if settings isn't in progress
	IS_SETTINGS=$( IsSettingsOpen )
	if [ "$IS_SETTINGS" = "YES" ]; then
		SupervisorLog "Settings activity in progress ..."
	else
		# Ensure the ehs app is running
		IS_RUNNING=$( IsAppRunning $EHS_PACKAGE )
		if [ "$IS_RUNNING" = "YES" ]; then
			SupervisorLog "EHS app  ==========> OK!"
		else
			SupervisorError "EHS app is NOT running!"
			SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"	
		fi
	fi	
}

RestartApp(){
	SupervisorLog "Restarting Apps ..."
	StopApp $EHS_PACKAGE
	SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
}

DevicePrefix(){
	echo "mEHS-"
}

AppEventsHandler(){
	# Override this function to handle any app specific events
}
