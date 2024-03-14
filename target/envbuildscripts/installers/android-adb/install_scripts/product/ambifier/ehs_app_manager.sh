#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_app_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_settings.sh"

EHS_PACKAGE="com.inx.ehs"
AMBIFIER_PACKAGE="com.TheSoundAgency.Ambifier2"
AMBIFIER_LAUNCH_DELAY=5
EHS_SWITCH_LOCK="$EHS_STORAGE_LOCATION/.ehs_switch_lock"

# this is called from the ehs supervisor
LockEhsSwitch(){
    LOCKED=$( Lock "$EHS_SWITCH_LOCK" )
    if [ -z "$LOCKED" ]; then
        SupervisorLog "EHS switch event [LOCK]"
    else
        SupervisorLog "EHS switch event lock ($LOCKED)"
    fi 
}

UnlockEhsSwitch(){
    UNLOCKED=$( Unlock "$EHS_SWITCH_LOCK" )
    if [ -z "$UNLOCKED" ]; then
        SupervisorLog "EHS switch event [UNLOCKED]"
    else
        SupervisorLog "EHS switch event unlock ($UNLOCKED)"
    fi 
}

EhsSwitchLocked(){
    Locked "$EHS_SWITCH_LOCK"
}

InitAppManger(){
    APK_INSTALL_DIR="/sdcard/.EHS_INSTALL"
    SERVER_ADDRESS_URL_DIR="/sdcard/Android/data/com.inx.ehs/files/devman/core/config"
    SERVER_ADDRESS_URL_FILE="$SERVER_ADDRESS_URL_DIR/DEVMANURL.000"
    REBOOT="No"
    INSTALLED=$( IsPackageInstalled $EHS_PACKAGE )
    # clear locks
    UnlockSettings
    UnlockEhsSwitch
    if [ -z "$INSTALLED" ]; then
        WaitDir $APK_INSTALL_DIR 10
		APK_NAME="ehs.apk"
        APK_LOCATION="$APK_INSTALL_DIR/$APK_NAME"
		InstallNewApk $APK_NAME $APK_LOCATION $EHS_PACKAGE
        if [ -n "$( IsPackageInstalled $EHS_PACKAGE )" ]; then
            REBOOT="Yes"
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
	INSTALLED=$( IsPackageInstalled $AMBIFIER_PACKAGE )
    if [ -z "$INSTALLED" ]; then
        REBOOT="No"
        WaitDir $APK_INSTALL_DIR 10
		APK_NAME="ambifier.apk"
        APK_LOCATION="$APK_INSTALL_DIR/$APK_NAME"
        InstallNewApk $APK_NAME $APK_LOCATION $AMBIFIER_PACKAGE
        if [ -n "$( IsPackageInstalled $AMBIFIER_PACKAGE )" ]; then
            REBOOT="Yes"
        fi
	fi
    if [ "$REBOOT" = "Yes" ]; then
        StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
        RebootDevice
    else
    	# Launch the applications	
    	SupervisorLog "Initialising ambifier."
    	StartHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
    	sleep $AMBIFIER_LAUNCH_DELAY
    	StartApp $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity"
    fi
}

RunAppManger(){
	# check if settings isn't in progress
	IS_SETTINGS=$( IsSettingsOpen )
	if [ "$IS_SETTINGS" = "YES" ]; then
		SupervisorLog "Settings activity in progress ..."
    elif [ "$( SettingsLocked )" = "YES" ]; then
        SupervisorLog "Settings activity locked ..."
    elif [ "$( EhsSwitchLocked )" = "YES" ]; then
        SupervisorLog "EHS switch locked ..."
        UnlockEhsSwitch
	else
		# ensure the ehs app is running
		IS_RUNNING=$( IsAppRunning $EHS_PACKAGE )
		if [ "$IS_RUNNING" = "YES" ]; then
			SupervisorLog "EHS app  ==========> OK!"
		else
			SupervisorError "EHS app is NOT running!"
			SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
			sleep $AMBIFIER_LAUNCH_DELAY
		fi	
		# ensure the ambifier app is in focus
		IS_ACTIVE=$( IsActivityFocused $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity" )
		if [ "$IS_ACTIVE" = "YES" ]; then
			SupervisorLog "Ambifier focus ====> OK!"
		else
			SupervisorError "Ambifier app is NOT focused!"
			LaunchApp $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity"
		fi	
	fi	
}

RestartApp(){
    SupervisorLog "Restarting Apps ..."
    LockSettings
	StopApp $AMBIFIER_PACKAGE
	StopApp $EHS_PACKAGE
	SetAsHomeApp $EHS_PACKAGE ".EhsHomeNativeActivity"
	sleep $AMBIFIER_LAUNCH_DELAY
	LaunchApp $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity"
    sleep 1
    UnlockSettings
}

DevicePrefix(){
	echo "mEHS-"
}

EHS_VIEW_TOGGLE_KEY_CODE="00070008" # "e"

AppEventsHandler(){
	EVENT=$1
	if [[ "$EVENT" == *"$EHS_VIEW_TOGGLE_KEY_CODE"* ]]; then
		IS_ACTIVE=$( IsActivityFocused $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity" )
		if [ "$IS_ACTIVE" = "YES" ]; then
			SupervisorLog "Switching to EHS app"
            LockEhsSwitch
			LaunchHomeApp
		else
			SupervisorLog "Switching to Ambifier app"
			LaunchApp $AMBIFIER_PACKAGE "com.unity3d.player.UnityPlayerActivity"
            sleep 1
            UnlockEhsSwitch
		fi
	fi
}
