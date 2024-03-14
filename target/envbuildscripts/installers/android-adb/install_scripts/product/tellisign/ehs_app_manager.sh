#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_app_utils.sh"

SSO_PACKAGE="online.show.signplayer"

# override
InitAppManger(){
    APK_INSTALL_DIR="/sdcard/.EHS_INSTALL"
    SERVER_ADDRESS_URL_DIR="/sdcard/Android/data/online.show.signplayer/files/devman/core/config"
    SERVER_ADDRESS_URL_FILE="$SERVER_ADDRESS_URL_DIR/DEVMANURL.000"
    REBOOT="No"
    INSTALLED=$( IsPackageInstalled $SSO_PACKAGE )
    # clear locks
    UnlockSettings
    if [ -z "$INSTALLED" ]; then
        WaitDir $APK_INSTALL_DIR 10
        APK_NAME="tellisign.apk"
        APK_LOCATION="$APK_INSTALL_DIR/$APK_NAME"
        InstallNewApk $APK_NAME $APK_LOCATION $SSO_PACKAGE
        if [ -n "$( IsPackageInstalled $SSO_PACKAGE )" ]; then
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
    if [ "$REBOOT" = "Yes" ]; then
      StartHomeApp $SSO_PACKAGE "com.unity3d.player.UnityPlayerHomeActivity"
      RebootDevice
    else
      # Launch the applications
      SupervisorLog "Initialising Tellisign."
      StartHomeApp $SSO_PACKAGE "com.unity3d.player.UnityPlayerHomeActivity"
    fi
}

# override
RunAppManger(){
  # check if settings isn't in progress
  IS_SETTINGS=$( IsSettingsOpen )
  if [ "$IS_SETTINGS" = "YES" ]; then
      SupervisorLog "Settings activity in progress ..."
  elif [ "$( SettingsLocked )" = "YES" ]; then
      SupervisorLog "Settings activity locked ..."
  else
    # ensure the ehs app is running
    IS_RUNNING=$( IsAppRunning $SSO_PACKAGE )
    if [ "$IS_RUNNING" = "YES" ]; then
      SupervisorLog "Tellisign app  ==========> OK!"
    else
      SupervisorError "Tellisign app is NOT running!"
      StartHomeApp $SSO_PACKAGE "com.unity3d.player.UnityPlayerHomeActivity"
    fi
  fi
}

RestartApp(){
  SupervisorLog "Restarting Apps ..."
  LockSettings
  StopApp $SSO_PACKAGE
  StartHomeApp $SSO_PACKAGE "com.unity3d.player.UnityPlayerHomeActivity"
  sleep 5
  UnlockSettings
}

# override
DevicePrefix(){
	echo "mSSO-"
}

AppEventsHandler(){
	# NONE
}
