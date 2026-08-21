#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

LaunchSettings(){
	am start -a android.settings.SETTINGS
}

CloseSettings(){
	StopApp com.android.settings
}

#TODO This should check for Both com.android.settings and com.android.tv.settings to be compatible with Androidtv and AOSP android
IsSettingsOpen(){
	IsPackageFocused com.android.settings
}
