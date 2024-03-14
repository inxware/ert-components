#!/system/bin/sh

DEVMAN_UPDATE_CLEAN_MEDIA_KEY="ehs-android-clear-media-script"
DEVMAN_UPDATE_MUTE_KEY="ehs-android-mute-script"
DEVMAN_UPDATE_REBOOT_KEY="ehs-android-reboot-script"
DEVMAN_UPDATE_RESTART_KEY="ehs-android-restart-script"
DEVMAN_UPDATE_TIME_ZONE_KEY="ehs-android-time-zone-script"
DEVMAN_UPDATE_UTC_DATE_KEY="ehs-android-utc-date-script"
DEVMAN_UPDATE_NTPURL_KEY="ehs-android-ntp-server-script"
DEVMAN_UPDATE_VOLUME_KEY="ehs-android-volume-script"
DEVMAN_UPDATE_LOGS0_KEY="ehs-android-logs0-script"
DEVMAN_UPDATE_LOGS1_KEY="ehs-android-logs1-script"

DEVMAN_UPDATE_PATCH_KEY="ehs-android-update-script"

HandleDevmanOperationUpdates(){
    if [ "$1" = "" ]; then 
    	EHS_SUPSCRIPTS_LOCATION=$EHS_UPDATES_LOCATION
    else
    	EHS_SUPSCRIPTS_LOCATION=$1
    fi
    SCRIPT_FILE="$EHS_SUPSCRIPTS_LOCATION/dldata.sh"
    if [ -f "$SCRIPT_FILE" ]; then
        FILE_CONTENT=$(cat "$SCRIPT_FILE")
        if [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_CLEAN_MEDIA_KEY"* ]]; then
            SupervisorLog "Clean media folder"
            rm -Rf /sdcard/Android/data/com.inx.ehs/files/userdata/media/*
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_MUTE_KEY"* ]]; then
            SupervisorLog "Setting volume mute "
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_REBOOT_KEY"* ]]; then
            SupervisorLog "Devman requested reboot ..."
            RebootDevice
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_TIME_ZONE_KEY"* ]]; then
            VALUE=$(cat "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz")
            SupervisorLog "Setting time-zone ($VALUE)"
            SetTimeZone "$VALUE"
            sleep 1
            SupervisorLog "Time Zone : $( GetTimeZone )"
        	SupervisorLog "UTC Date  : $( GetUTCDate )"
        	SupervisorLog "Date      : $( GetDate )"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_UTC_DATE_KEY"* ]]; then
            VALUE=$(cat "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz")
            SupervisorLog "Setting UTC date ($VALUE)"
            SetUTCDate "$VALUE"
            sleep 1
            SupervisorLog "Time Zone : $( GetTimeZone )"
        	SupervisorLog "UTC Date  : $( GetUTCDate )"
        	SupervisorLog "Date      : $( GetDate )"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_VOLUME_KEY"* ]]; then
            VALUE=$(cat "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz")
            SupervisorLog "Setting master volume ($VALUE)"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_NTPURL_KEY"* ]]; then
            VALUE=$(cat "$EHS_SUPSCRIPTS_LOCATION/dldata.tgz")
            SetNtpServer "$VALUE"
            SupervisorLog "Setting NTP server to ($VALUE)"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_LOGS0_KEY"* ]]; then
            SupervisorLog "Update logs0 ==> $EHS_SUPSCRIPTS_LOCATION ==> $SERVER_ADDRESS ==> $EHS_DEVICE_ID"
            /system/bin/sh ${EHS_SUPERVISOR_LOCATION}/ehs_upload_logs.sh "0" "$EHS_SUPSCRIPTS_LOCATION" "$SERVER_ADDRESS" "$EHS_DEVICE_ID"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_LOGS1_KEY"* ]]; then
            SupervisorLog "Update logs1 ==> $EHS_SUPSCRIPTS_LOCATION ==> $SERVER_ADDRESS ==> $EHS_DEVICE_ID"
            /system/bin/sh ${EHS_SUPERVISOR_LOCATION}/ehs_upload_logs.sh "1" "$EHS_SUPSCRIPTS_LOCATION" "$SERVER_ADDRESS" "$EHS_DEVICE_ID"
        elif [[ "$FILE_CONTENT" == *"$DEVMAN_UPDATE_PATCH_KEY"* ]]; then
            echo "patch"
        fi
    else
        SupervisorLog "Failed to perform devman operation. Updates file not available!!!"
    fi
}
