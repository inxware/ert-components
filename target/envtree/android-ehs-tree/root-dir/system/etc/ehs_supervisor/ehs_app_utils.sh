#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

SETTINGS_LOCK="$EHS_STORAGE_LOCATION/.settings_lock"

StartHomeApp(){
    PACKAGE=$1
    INTENT=$2
    IS_INSTALLED=$( IsPackageInstalled $PACKAGE )
    if [ "$IS_INSTALLED" = "YES" ]; then
        IS_HOME_APP=$( IsHomeApp $PACKAGE )
        if [ -z "$IS_HOME_APP" ]; then
            SupervisorLog "Setting-up the app ($PACKAGE) as a home app."
            MSG=$( SetAsHomeApp $PACKAGE $INTENT )
            SupervisorLog "SetAsHomeApp ====> $MSG"  
        fi
        # confirm the app is running
        DELAY=5; LOGGED="";
        while true; do
            sleep $DELAY
    	    IS_ACTIVE=$( IsActivityFocused $PACKAGE $INTENT )
    		if [ "$IS_ACTIVE" = "YES" ]; then
    			SupervisorLog "Successful application ($PACKAGE) launch."
    			break
            else
                if [ -z "$LOGGED" ]; then
                    SupervisorError "Failed to run the app. Retrying ..."
                    FOCUSED=$( GetFocusedActivity )
                    SupervisorLog "Active package is ($FOCUSED)"
                    LOGGED="Yes"
                fi
                IS_HOME_APP=$( IsHomeApp $PACKAGE )
                if [ "$IS_HOME_APP" = "YES" ]; then
                    $( LaunchHomeApp )
                else
                    MSG=$( SetAsHomeApp $PACKAGE $INTENT )
                fi
    		fi
        done
    else
        SupervisorError "Package ($PACKAGE) is not installed."    
    fi    
}

StartApp(){
    PACKAGE=$1
    INTENT=$2
    IS_INSTALLED=$( IsPackageInstalled $PACKAGE )
    if [ "$IS_INSTALLED" = "YES" ]; then
        LaunchApp $PACKAGE $INTENT
        # confirm the app is running
        DELAY=5; LOGGED="";
        while true; do
            sleep $DELAY
    	    IS_ACTIVE=$( IsActivityFocused $PACKAGE $INTENT )
    		if [ "$IS_ACTIVE" = "YES" ]; then
    			SupervisorLog "Successful application ($PACKAGE) launch."
    			break
            else
                if [ -z "$LOGGED" ]; then
                    SupervisorError "Failed to run the app. Retrying ..."
                    FOCUSED=$( GetFocusedActivity )
                    SupervisorLog "Active package is ($FOCUSED)"
                    LOGGED="Yes"
                fi
                LaunchApp $PACKAGE $INTENT
    		fi
        done
    else
        SupervisorError "Package ($PACKAGE) is not installed."   
    fi    
}

DownloadApk(){
    APK_NAME=$1
    APK_LOCATION=$2
    ADDRESS=$3
    ADDRESS_PATH=$4
    SupervisorLog 'Download address ('${ADDRESS}${ADDRESS_PATH}')'
    MSG=$( LaunchDownloaderSetupPage ) # launching a set-up intent
    SupervisorLog "$MSG"
    if ! [ -f "$APK_LOCATION" ]; then
        SupervisorLog "Downloading ($APK_NAME) ==> ($APK_LOCATION) ..."
        DownloaderStatus "Downloading ($APK_NAME) ..."
        DELAY=3;LOGGED="";RETRIES=3;
        while true; do
            COMPLETE=$( Downloader "$ADDRESS" "$ADDRESS_PATH" "$APK_LOCATION" )
            if [ "$COMPLETE" = "YES" ]; then
                SupervisorLog "Successful download of ($APK_NAME) apk."
                break
            else
                if [ -z "$LOGGED" ]; then
                    SupervisorError "Failed to download ($APK_NAME) apk. ====> $COMPLETE"
                fi
            fi
            sleep $DELAY
            if [ "$RETRIES" -lt "1" ]; then
                # failed to download the app
                DownloaderStatus "Failed to download $ADDRESS // $ADDRESS_PATH // $APK_LOCATION Is the network configured?"
                SupervisorLog "Failed to download. $ADDRESS // $ADDRESS_PATH // $APK_LOCATION Is the network configured?"
            else
                RETRIES=$((RETRIES-1))
            fi
            if [ -z "$LOGGED" ]; then
                SupervisorLog "Retrying ..."
                LOGGED="YES"
            fi
        done
    fi
}

InstallNewApk(){
    APK_NAME=$1
    APK_LOCATION=$2
    PACKAGE=$3
    MSG=$( LaunchDownloaderSetupPage ) # launching a set-up intent
    SupervisorLog "$MSG"
    DownloaderStatus "Installing ($APK_NAME) ..."
    sleep 2
    if [ -f "$APK_LOCATION" ]; then
        VALID=$( IsApkValid $APK_LOCATION )
        if [ -z "$VALID" ]; then
            MSG=$( InstallApp $APK_LOCATION )
            SupervisorLog "InstallApp ====> $MSG"
            sleep 1
            INSTALLED=$( IsPackageInstalled $PACKAGE )
            if [ "$INSTALLED" = "YES" ]; then
                SupervisorLog "The installation was successful ($APK_NAME)."
                DownloaderStatus "Success!"
            else
                DownloaderStatus "Failed to install!"
                SupervisorLog "The installation failed ($APK_NAME)."
            fi
            sleep 1
        else
            SupervisorError "Invalid ($APK_NAME) apk."
        fi
        rm $APK_LOCATION
    else
        DownloaderStatus "Failed to install!"
        SupervisorError "Failed to install ($APK_NAME) apk."
        sleep 1
    fi
}

LockSettings(){
    LOCKED=$( Lock "$SETTINGS_LOCK" )
    if [ -z "$LOCKED" ]; then
        SupervisorLog "Settings event [LOCK]"
    else
        SupervisorLog "Settings event lock ($LOCKED)"
    fi 
}

UnlockSettings(){
    UNLOCKED=$( Unlock "$SETTINGS_LOCK" )
    if [ -z "$UNLOCKED" ]; then
        SupervisorLog "Settings event [UNLOCKED]"
    else
        SupervisorLog "Settings event unlock ($UNLOCKED)"
    fi 
}

SettingsLocked(){
    Locked "$SETTINGS_LOCK"
}
