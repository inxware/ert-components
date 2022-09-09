#!/system/bin/sh

export EHS_SUPERVISOR_LOCATION="/system/etc/ehs_supervisor"
export DEVICE_STORAGE_LOCATION="/storage/emulated/0"
export EHS_STORAGE_LOCATION="$DEVICE_STORAGE_LOCATION/.EHS"
export EHS_APP_STORAGE_LOCATION="$DEVICE_STORAGE_LOCATION/Android/data/com.inx.ehs/files/"
export EHS_UPDATES_LOCATION="$EHS_STORAGE_LOCATION/Updates"

EHS_SUPERVISOR_SLEEP=5 # [sec]
EHS_SUPERVISOR_UPDATES_TIMER=30 # [sec]
EHS_SUPERVISOR_MANAGER_TIMER=15 # [sec]

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_server_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_gpio_setup.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_devman_operations.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_id_gen.sh"

ReadDeviceId(){
	FILE="$EHS_STORAGE_LOCATION/.ehs"
	if [ -f "$FILE" ]; then
		echo "$(cat $FILE)" 
	fi
}

ClearUpdatesDir(){
	if ! [ -z "$EHS_UPDATES_LOCATION" ]; then
		if [ -d "$EHS_UPDATES_LOCATION" ]; then
			IS_EMPTY=$( ls -A $EHS_UPDATES_LOCATION )
			if ! [ -z "$IS_EMPTY" ]; then
				rm -rf $EHS_UPDATES_LOCATION/*
			fi
		fi
	fi
}

HandleDeviceId(){
	LOGGED=""
	while true; do
		EhsSetupId
		if [ -z "$EHS_DEVICE_ID" ]; then
			READ_ID=$(ReadDeviceId)
			if [ -z "$READ_ID" ]; then
				EhsOverwriteId
				READ_ID=$(ReadDeviceId)
				if [ -z "$READ_ID" ]; then
					if [ -z "$LOGGED" ]; then
						SupervisorError "Failed to read device ID."
						LOGGED="YES"
					fi
					sleep 1
					continue
				fi
			fi
			PREFIX=$( DevicePrefix )
			export EHS_DEVICE_ID=${PREFIX}${READ_ID}
		fi
		SupervisorLog "Device ID ===> OK"
		break
	done
	EHS_ID=$( EhsReadId )
	EHS_LOCAL_ID=$( ReadDeviceId )
	if [ "$EHS_LOCAL_ID" != "$EHS_ID" ]; then
		SupervisorLog "Override local id as it dosen't match the main id."
		EhsOverwriteId
	fi
}

EhsSetup(){
	SUPERVISOR_VERSION=$( cat "$EHS_SUPERVISOR_LOCATION/version" )
	SupervisorLog "Setting-up the supervisor (v$SUPERVISOR_VERSION)."

	# wait for the sd storage to be accesible
	WaitDir "$DEVICE_STORAGE_LOCATION" 500 # 500 [sec]

	SupervisorLog "Folders set-up."

	if ! [ -d "$EHS_STORAGE_LOCATION" ]; then
		mkdir $EHS_STORAGE_LOCATION
	fi
	if ! [ -d "$EHS_UPDATES_LOCATION" ]; then
		mkdir $EHS_UPDATES_LOCATION
	fi
	
	HandleDeviceId

	SupervisorLog "********* EHS Supervisor ($EHS_DEVICE_ID) ***********"
	
	# Display device time info 
	SupervisorLog "Time Zone : $( GetTimeZone )"
	SupervisorLog "UTC Date  : $( GetUTCDate )"
	SupervisorLog "Date      : $( GetDate )"
	
	ClearUpdatesDir
	
	SetupDeviceGpio
	
	InitAppManger

	SupervisorLog "Devman address is $SERVER_ADDRESS"

	# This script will control the settings page
	${EHS_SUPERVISOR_LOCATION}/ehs_event_handler.sh &
}

EhsAppManager(){
	# run the app manager, which is specific to the product and
	# should be implemanted in 'ehs_app_manager.sh' script  
	RunAppManger
}

DevmanDownloader(){
	ADDRESS=$1; ID=$2; OUTPUT_PATH=$3;
	if ! [ -z "$ADDRESS" ]; then
		if ! [ -z "$OUTPUT_PATH" ]; then
			# run downloader
			MSG=$( InitDownloader )
			$EHS_SUPERVISOR_LOCATION/ehs_downloader.sh "$ADDRESS" "$ID" "$OUTPUT_PATH"
		else
			echo "Output path was not specified."
		fi
	else
		echo "IP address was not specified."
	fi
}

UpdatesReady(){
	DEVICE_ID=$1; ADDRESS=$2;
	# run downloader
	ClearUpdatesDir
	if ! [ -d "$EHS_UPDATES_LOCATION" ]; then
		mkdir $EHS_UPDATES_LOCATION
	fi
	COMPLETE=$( DevmanDownloader "$ADDRESS" "$DEVICE_ID" "$EHS_UPDATES_LOCATION" )
	if [ "$COMPLETE" = "YES" ]; then
		echo "YES"
	else
		ClearUpdatesDir
	fi
}

EhsUpdater(){
	ADDRESS=$1
	UPDATE_TYPE=""
	READY=$( UpdatesReady $EHS_DEVICE_ID $ADDRESS )
	if [ "$READY" = "YES" ]; then
		UPDATE_TYPE=$( HandleDevmanOperationUpdates )
	fi
	if [ "$UPDATE_TYPE" = "patch" ]; then
		REBOOT="No"
		SupervisorLog "========== START UPDATE =========="
		LaunchDownloaderUpdatePage
		DownloaderStatus "Software update in progress. Please wait ..."
		UPDATE_SCRIPT="$EHS_UPDATES_LOCATION/dldata.sh"
		if [ -f "$UPDATE_SCRIPT" ]; then
			SupervisorLog "Updates script ready."
			chmod 0755 $UPDATE_SCRIPT
			chown "shell:root" $UPDATE_SCRIPT
			# run update
			SCRIPT_LOGS=$( /system/bin/sh $UPDATE_SCRIPT $EHS_UPDATES_LOCATION )
			SupervisorLog "$SCRIPT_LOGS"
			REBOOT="Yes"
		else
			SupervisorLog "FAILED to download the update script."
		fi
		ClearUpdatesDir
		SupervisorLog "========== END UPDATE =========="
		if [ "$REBOOT" = "Yes" ]; then
	        	RebootDevice
		else
			CloseDownloaderUpdatePage	
	    fi
	fi
}

EhSuperCommands(){
	EHS_SUPSCRIPTS_LOCATION="${EHS_APP_STORAGE_LOCATION}/userdata/platform/"
	if [ -f "$EHS_SUPSCRIPTS_LOCATION/dldata.sh" ]; then
		REBOOT="No"
		UPDATE_TYPE=$( HandleDevmanOperationUpdates "$EHS_SUPSCRIPTS_LOCATION" )
		if [ "$UPDATE_TYPE" = "patch" ]; then
			SupervisorLog "========== START UPDATE =========="
			UPDATE_SCRIPT="$EHS_SUPSCRIPTS_LOCATION/dldata.sh"
			if [ -f "$UPDATE_SCRIPT" ]; then
				SupervisorLog "EHS superscript ready."
				chmod 0755 $UPDATE_SCRIPT
				chown "shell:root" $UPDATE_SCRIPT
				# run update
				SCRIPT_LOGS=$( /system/bin/sh $UPDATE_SCRIPT $EHS_SUPSCRIPTS_LOCATION )
				SupervisorLog "$SCRIPT_LOGS"
				REBOOT="Yes"
			else
				SupervisorLog "FAILED to download the update script."
			fi
			SupervisorLog "========== END UPDATE =========="
			
		fi
		test -f "$EHS_SUPSCRIPTS_LOCATION/dldata.sh" && rm "$EHS_SUPSCRIPTS_LOCATION/dldata.sh"
		test -f "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz" && rm "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz"
		if [ "$REBOOT" = "Yes" ]; then
			RebootDevice
		fi
	fi
}

#################################################################
############################ RUN ################################
#################################################################

EhsSetup

UPDATE_COUNTER_MAX=$((EHS_SUPERVISOR_UPDATES_TIMER/EHS_SUPERVISOR_SLEEP))
UPDATE_COUNTER=0
MANAGE_COUNTER_MAX=$((EHS_SUPERVISOR_MANAGER_TIMER/EHS_SUPERVISOR_SLEEP))
MANAGE_COUNTER=0

while true
do
	sleep $EHS_SUPERVISOR_SLEEP
	
	# manage the updates
	if [ "$UPDATE_COUNTER" = "$UPDATE_COUNTER_MAX" ]; then
		EhsUpdater $SERVER_ADDRESS
		EhSuperCommands
		UPDATE_COUNTER=0
	fi

	# manage the apps
	if [ "$MANAGE_COUNTER" = "$MANAGE_COUNTER_MAX" ]; then
		EhsAppManager
		MANAGE_COUNTER=0
	fi

	MANAGE_COUNTER=$((MANAGE_COUNTER+1))
	UPDATE_COUNTER=$((UPDATE_COUNTER+1))
done
