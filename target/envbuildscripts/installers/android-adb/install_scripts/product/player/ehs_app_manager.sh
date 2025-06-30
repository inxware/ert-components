#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_app_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_settings.sh"

EHS_PACKAGE="com.inx.ehs"

InitAppManger(){
    APK_INSTALL_DIR="/sdcard/.EHS_INSTALL"
    SERVER_ADDRESS_URL_DIR="/sdcard/Android/data/com.inx.ehs/files/devman/core/config"
    SERVER_ADDRESS_URL_FILE="$SERVER_ADDRESS_URL_DIR/DEVMANURL.000"
    REBOOT="No"
    INSTALLED=$( IsPackageInstalled $EHS_PACKAGE )
    # clear locks
    UnlockSettings
    if [ -z "$INSTALLED" ]; then
        SupervisorLog "Package ($EHS_PACKAGE) not installed ..."
        WaitDir $APK_INSTALL_DIR 60
		APK_NAME="ehs.apk"
        APK_LOCATION="$APK_INSTALL_DIR/$APK_NAME"
        if [ -f "$APK_LOCATION" ]; then
            SupervisorLog "Installing ($APK_LOCATION)  ..."
            InstallNewApk $APK_NAME $APK_LOCATION $EHS_PACKAGE
            CopyEhsDeviceID "$( ReadDeviceId )"
            if [ -n "$( IsPackageInstalled $EHS_PACKAGE )" ]; then
                REBOOT="Yes"
            fi
        fi
    else
        WaitDir $SERVER_ADDRESS_URL_DIR 10  
        if [ -f "$SERVER_ADDRESS_URL_FILE" ]; then
            URL=$( cat $SERVER_ADDRESS_URL_FILE )
            if [ -n "$URL" ]; then
                SupervisorLog "Setting url ("$URL")"
                # is this the intial installer server, which might be different to the in-life update server?
                export SERVER_ADDRESS="$URL"
            else
                SupervisorLog "Url file is empty ("$SERVER_ADDRESS_URL_FILE")"
            fi
        else
            SupervisorLog "Url file is not present ("$SERVER_ADDRESS_URL_FILE")"
        fi    
	fi
    if [ "$REBOOT" = "Yes" ]; then
        StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
        RebootDevice
    else
    	# Launch the applications
    	SupervisorLog "Initialising EHS Player."
    	StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
    fi
}

RunAppManger(){
	# check if settings isn't in progress
	IS_SETTINGS=$( IsSettingsOpen )
	if [ "$IS_SETTINGS" = "YES" ]; then
		SupervisorLog "Settings activity in progress ..."
    elif [ "$( SettingsLocked )" = "YES" ]; then
        SupervisorLog "Settings activity locked ..."
	else
		# ensure the ehs app is running
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
    LockSettings
	StopApp $EHS_PACKAGE
	SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
    sleep 5
    UnlockSettings
}

DevicePrefix(){
	echo "mEHS-"
}

AppEventsHandler(){
	# Override this function to handle any app specific events
}
