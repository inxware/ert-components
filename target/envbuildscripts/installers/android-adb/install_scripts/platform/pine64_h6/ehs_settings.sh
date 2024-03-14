#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

LaunchSettings(){
	am start -a android.settings.SETTINGS
}

CloseSettings(){
	StopApp com.android.tv.settings
}

IsSettingsOpen(){
	IsPackageFocused com.android.tv.settings
}
