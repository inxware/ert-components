#!/system/bin/sh

export EHS_SUPERVISOR_LOCATION="/system/etc/ehs_supervisor"
export DEVICE_STORAGE_LOCATION="/storage/emulated/0"
export EHS_APP_STORAGE_LOCATION="$DEVICE_STORAGE_LOCATION/Android/data/com.inx.ehs/files"
export DOWNLOADER_STORAGE_LOCATION="$DEVICE_STORAGE_LOCATION/Android/data/com.utils.downloader/files"
export EHS_UPDATES_LOCATION="$DOWNLOADER_STORAGE_LOCATION"
export EHS_STORAGE_LOCATION="$DEVICE_STORAGE_LOCATION/.EHS"

# Polling rate of supervisor (seconds)
EHS_SUPERVISOR_SLEEP=5
# Delay before applying supervisor updates (seconds)
EHS_SUPERVISOR_UPDATES_TIMER=30
# Interval for doing supervisor checks (seconds)
EHS_SUPERVISOR_MANAGER_TIMER=15
# Time out (seconds)
EHS_SUPERVISOR_LOCK_TIMEOUT=600

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_server_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_gpio_setup.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_devman_operations.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_id_gen.sh"

# lock and flag files
EHS_SUPERVISOR_LOCK="$EHS_STORAGE_LOCATION/.ehs_supervisor.lock"
EHS_NETWORK_RESTART="$DOWNLOADER_STORAGE_LOCATION/ehs_network_restart.flag"

# if this file exists, it will be used to overwrite existing device id with its content
EHS_NEW_DEVICE_ID_OVERWITE="$EHS_STORAGE_LOCATION/ehs_new_id"
# this contains new device id and is used as flag to update it
export EHS_NEW_DEVICE_ID_REQUEST_FILE="$EHS_STORAGE_LOCATION/ehs_requested_new_id"

LockSupervisor(){
	SupervisorLog "Supervisor Lock ON"
	echo "ON" > $EHS_SUPERVISOR_LOCK
}

UnlockSupervisor(){
	SupervisorLog "Supervisor Lock OFF"
	test -f "$EHS_SUPERVISOR_LOCK" && rm "$EHS_SUPERVISOR_LOCK"
}

IsSupervisorLocked(){
	test -f "$EHS_SUPERVISOR_LOCK" && cat "$EHS_SUPERVISOR_LOCK"
}

IsNetworkRestartRequested(){
	test -f "$EHS_NETWORK_RESTART" && echo "YES"
}

ClearNetworkRestartFlag(){
	test -f "$EHS_NETWORK_RESTART" && rm "$EHS_NETWORK_RESTART"
}

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

	export EHS_DEVICE_ID=""

	# check if device id overwrite file exitst
	if [ -f "$EHS_NEW_DEVICE_ID_OVERWITE" ]; then
		SupervisorLog "Overwriting device ID with id send from devman"
		cat $EHS_NEW_DEVICE_ID_OVERWITE > $EHS_STORAGE_LOCATION/.ehs
		READ_ID=$( ReadDeviceId )
		if [ -n "$READ_ID" ]; then
			PREFIX=$( DevicePrefix )
			export EHS_DEVICE_ID=${PREFIX}${READ_ID}
		fi
	fi

	# read device id if not set up
	if [ -z "$EHS_DEVICE_ID" ]; then

	LOGGED=""
	for i in $(seq 1 10); do # do max 10 attempts
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
		SupervisorLog "Override local ($EHS_LOCAL_ID) id as it dosen't match the device id ($EHS_ID)."
		EhsOverwriteId
		READ_ID=$( ReadDeviceId )
		if [ -n "$READ_ID" ]; then
			PREFIX=$( DevicePrefix )
			export EHS_DEVICE_ID=${PREFIX}${READ_ID}
		fi
	fi

	fi
	if [ -z "$EHS_DEVICE_ID" ]; then
		SupervisorError "Failed to read device ID !!!!!"
		# todo - set recovery device id. random generated with prefix?
	else
		# make sure that id is also copied to the eRT app external storage location 
		# this is needed for Android 11+, as the apps cannot read data from elsewhere
		if [ -n "$( IsPackageInstalled 'com.inx.ehs' )" ]; then
			CopyEhsDeviceID "$( ReadDeviceId )"
		fi
	fi
}

HandleNewDeviceIdRequest(){
	if [ -f "$EHS_NEW_DEVICE_ID_REQUEST_FILE" ]; then
		mv $EHS_NEW_DEVICE_ID_REQUEST_FILE $EHS_NEW_DEVICE_ID_OVERWITE
		HandleDeviceId # this should update to a new id
		RestartApp     # restart apps for ert to pickup new id
	fi
}

#Runs at boot and cleans up the file system and creates some start logs
EhsSetup(){

	SupervisorLog "************** [Start EHS Supervisor] **************"

	# @TODO - use a better approach to see if the storage has been mounted
	# wait for the sd storage to be accesible. Make sure 'Android' directory exists so we know it's mounted 
	WaitDir "$DEVICE_STORAGE_LOCATION/Android" 500 # 500 [sec]

	SUPERVISOR_VERSION=$( cat "$EHS_SUPERVISOR_LOCATION/version" )
	SupervisorLog "Setting-up the supervisor (v$SUPERVISOR_VERSION)."

	SupervisorLog "Folders set-up."

	if ! [ -d "$EHS_STORAGE_LOCATION" ]; then
		SupervisorLog "Create $EHS_STORAGE_LOCATION ..."
		mkdir $EHS_STORAGE_LOCATION
	else
		SupervisorLog "$EHS_STORAGE_LOCATION exists"
	fi
	
	HandleDeviceId

	SupervisorLog "********* EHS Supervisor ($EHS_DEVICE_ID) ***********"
	
	# Display device time info 
	SupervisorLog "Time Zone : $( GetTimeZone )"
	SupervisorLog "UTC Date  : $( GetUTCDate )"
	SupervisorLog "Date      : $( GetDate )"
	
	ClearUpdatesDir
	
	SetupDeviceGpio

	ClearRebootFlag
	
	InitAppManger

	UnlockSupervisor

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
			SupervisorLog "InitDownloader status=${MSG}"
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
		MakeUpdatesDir
	fi
	COMPLETE=$( DevmanDownloader "$ADDRESS" "$DEVICE_ID" "$EHS_UPDATES_LOCATION" )
	if [ "$COMPLETE" = "YES" ]; then
		echo "YES"
	else
		ClearUpdatesDir
	fi
}

# obtains dldata scripts and tarballs from Devman (e.g. updates) then runs them 
EhsUpdater(){
	ADDRESS=$1
	UPDATE_TYPE=""
	READY=$( UpdatesReady $EHS_DEVICE_ID $ADDRESS )
	if [ "$READY" = "YES" ]; then
		LockSupervisor
		UPDATE_TYPE=$( HandleDevmanOperationUpdates )
		HandleNewDeviceIdRequest
		UnlockSupervisor
	fi
	if [ "$UPDATE_TYPE" = "patch" ]; then
		LockSupervisor
		REBOOT="No"
		SupervisorLog "========== START EHS UPDATE =========="
		# launch Downloader GUI page to display pending updates
		LaunchDownloaderUpdatePage
		DownloaderStatus "Software update in progress. Please wait ..."
		UPDATE_SCRIPT="$EHS_UPDATES_LOCATION/dldata.sh"
		if [ -f "$UPDATE_SCRIPT" ]; then
			SupervisorLog "Updates script ready."
			#Note the following doesn't usually work on sdcard
			chmod 0755 $UPDATE_SCRIPT
			chown "shell:root" $UPDATE_SCRIPT
			# run update (use the sh command to avoid previously mentioned executable limitation)
			SupervisorLog "DEVMAN UPDATE LOGS (BEGIN)"
			SCRIPT_LOGS=$( /system/bin/sh $UPDATE_SCRIPT $EHS_UPDATES_LOCATION )
			SupervisorLog "DEVMAN UPDATE LOGS:$SCRIPT_LOGS"
			SupervisorLog "DEVMAN UPDATE LOGS (END)"
		else
			SupervisorLog "FAILED to download the update script."
		fi
		ClearUpdatesDir
		SupervisorLog "========== END EHS UPDATE =========="
		CloseDownloaderUpdatePage
		UnlockSupervisor
	fi
}

# this reads and runs scripts which were downloaded by the eRT app
EhsSuperCommands(){
	EHS_SUPSCRIPTS_LOCATION="${EHS_APP_STORAGE_LOCATION}/userdata/platform/"
	if [ -f "$EHS_SUPSCRIPTS_LOCATION/dldata.sh" ]; then
		LockSupervisor
		UPDATE_TYPE=$( HandleDevmanOperationUpdates "$EHS_SUPSCRIPTS_LOCATION" )
		HandleNewDeviceIdRequest
		if [ "$UPDATE_TYPE" = "patch" ]; then
			SupervisorLog "========== START EHS COMMAND =========="
			UPDATE_SCRIPT="$EHS_SUPSCRIPTS_LOCATION/dldata.sh"
			if [ -f "$UPDATE_SCRIPT" ]; then
				SupervisorLog "EHS superscript ready."
				chmod 0755 $UPDATE_SCRIPT
				chown "shell:root" $UPDATE_SCRIPT
				# run update
				SCRIPT_LOGS=$( /system/bin/sh $UPDATE_SCRIPT $EHS_SUPSCRIPTS_LOCATION )
				SupervisorLog "$SCRIPT_LOGS"
			else
				SupervisorLog "FAILED to download the update script."
			fi
			SupervisorLog "========== END EHS COMMAND =========="
			
		fi
		test -f "$EHS_SUPSCRIPTS_LOCATION/dldata.sh" && rm "$EHS_SUPSCRIPTS_LOCATION/dldata.sh"
		test -f "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz" && rm "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz"
		UnlockSupervisor
	fi
}

CheckEhsRebootFlag(){
	REBOOT=$( IsRebootFlag )
	if [ "$REBOOT" = "yes" ]; then
		SupervisorLog "Reboot flag identified. Shutdown Supervisor."
		ClearRebootFlag
		UPDATE_PROCESS=$1
		if ! [ "$OTHER_PROCESS" = "" ]; then
			SupervisorLog "About to kill the update process ($UPDATE_PROCESS)"
			kill $UPDATE_PROCESS
		fi
		am start -a android.intent.action.REBOOT
		exit 0
	fi
}

#################################################################
########################### SETUP ###############################
#################################################################

EhsSetup

#################################################################
############################ RUN ################################
#################################################################

# create and run devman update process
{
	SupervisorLog "Run devman update process"

	UPDATE_COUNTER_MAX=$((EHS_SUPERVISOR_UPDATES_TIMER/EHS_SUPERVISOR_SLEEP))
	UPDATE_COUNTER=0

	while true
	do

	sleep $EHS_SUPERVISOR_SLEEP

	# manage the updates
	if [ "$UPDATE_COUNTER" -ge "$UPDATE_COUNTER_MAX" ]; then
		EhsUpdater $SERVER_ADDRESS
		EhsSuperCommands
		UPDATE_COUNTER=0
	fi

	UPDATE_COUNTER=$((UPDATE_COUNTER+1))
	done

}&

EHS_DEVMAN_UPDATE_PROCESS=$!

MANAGE_COUNTER_MAX=$((EHS_SUPERVISOR_MANAGER_TIMER/EHS_SUPERVISOR_SLEEP))
MANAGE_COUNTER=0
EHS_SUPERVISOR_LOCK_COUNTER_TIMEOUT=$((EHS_SUPERVISOR_LOCK_TIMEOUT/EHS_SUPERVISOR_SLEEP))
EHS_SUPERVISOR_LOCK_COUNTER=0


# run application manager loop
SupervisorLog "Run application manager loop"

while true
do
	sleep $EHS_SUPERVISOR_SLEEP
	
	EHS_SUPERVISOR_LOCKED=$( IsSupervisorLocked )
	if [ "$EHS_SUPERVISOR_LOCKED" = "" ]; then # check if the supervisor lock is OFF
		# manage the apps after the boot management period is done
		if [ "$MANAGE_COUNTER" -ge "$MANAGE_COUNTER_MAX" ]; then
			EhsAppManager
			MANAGE_COUNTER=0
		fi
		CheckEhsRebootFlag "$EHS_DEVMAN_UPDATE_PROCESS"
		EHS_SUPERVISOR_LOCK_COUNTER=0
	else
		SupervisorLog "Supervisor LOCK is ON."
        if [ "$EHS_SUPERVISOR_LOCK_COUNTER" -ge "$EHS_SUPERVISOR_LOCK_COUNTER_TIMEOUT" ]; then
			SupervisorLog "Supervisor lock timed out!!!!"
			UnlockSupervisor
			EHS_SUPERVISOR_LOCK_COUNTER=0
		else 
			EHS_SUPERVISOR_LOCK_COUNTER=$((EHS_SUPERVISOR_LOCK_COUNTER+1))
			SupervisorLog "Supervisor lock counter $EHS_SUPERVISOR_LOCK_COUNTER"
		fi
	fi

	if [ "$( IsNetworkRestartRequested )" = "YES" ]; then
		SupervisorLog "Network restart requested"
		RestartNetwork
		ClearNetworkRestartFlag
	fi
	
	MANAGE_COUNTER=$((MANAGE_COUNTER+1))
done
