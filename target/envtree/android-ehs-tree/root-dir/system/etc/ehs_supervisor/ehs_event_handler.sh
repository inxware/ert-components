#!/system/bin/sh

SETTINGS_TOGGLE_KEY_CODE="00070016" # "s"
VOLUME_UP_TOGGLE_KEY_CODE="00070010" # "m"
VOLUME_DOWN_TOGGLE_KEY_CODE="00070011" # "n"

SETTINGS_KEY_MODIFIER="000700e0" #l-ctrl

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_settings.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"

RunEventHandler(){
	SupervisorLog "Start Event Handler"
	while true
	do
		EVENT=$( getevent -q -l -c 4 )
		# launch settings
		if [[ "$EVENT" == *"$SETTINGS_TOGGLE_KEY_CODE"* ]] && 
		   [[ "$EVENT" == *"$SETTINGS_KEY_MODIFIER"* ]]; then
			IS_SETTINGS=$( IsSettingsOpen )
			if [ "$IS_SETTINGS" = "YES" ]; then
				# close settings
				SupervisorLog "Settings OFF"
				CloseSettings
				RestartApp
			else
				# turn on settings
				SupervisorLog "Settings ON"
				LaunchSettings
			fi
		else
			IS_SETTINGS=$( IsSettingsOpen )
			if [ "$IS_SETTINGS" != "YES" ]; then
				if [[ "$EVENT" == *"$VOLUME_UP_TOGGLE_KEY_CODE"* ]] && 
				   [[ "$EVENT" == *"$SETTINGS_KEY_MODIFIER"* ]]; then
					VolumeUp
				elif [[ "$EVENT" == *"$VOLUME_DOWN_TOGGLE_KEY_CODE"* ]] && 
				     [[ "$EVENT" == *"$SETTINGS_KEY_MODIFIER"* ]]; then
					VolumeDown	
				else
					AppEventsHandler "$EVENT"
				fi
			fi			
		fi
		sleep 0.5
	done
}


RunEventHandler
