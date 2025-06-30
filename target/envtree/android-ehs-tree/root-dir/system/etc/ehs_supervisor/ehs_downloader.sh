#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

ADDRESS=$1 
ID=$2
OUT_DIR=$3

LONG_DOWNLOAD_LOCK_FILE="/storage/emulated/0/Android/data/com.utils.downloader/files/downloading.lock"

# clear lock
test -f "$LONG_DOWNLOAD_LOCK_FILE" && rm "$LONG_DOWNLOAD_LOCK_FILE"

RESULT=$(am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD --es "ip_address" "$ADDRESS" \
 --es "check_available" /cgi-bin/isupdated.cgi --es "available_response" "data_ready" --es "available_post" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" \
 --es "download_path0" "/DeviceGroup/DEFAULT/DEFAULT/$ID/dldata.sh" --es "output_path0" "$OUT_DIR/dldata.sh" --es "ignore_failed0" "No" \
 --es "download_path1" "/DeviceGroup/DEFAULT/DEFAULT/$ID/dldata.tgz" --es "output_path1" "$OUT_DIR/dldata.tgz" --es "ignore_failed1" "Yes" \
 --es "pre_download_post" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" --es "post_path" "/cgi-bin/downloading.cgi")
 
if [[ "$RESULT" == *"result=100"* ]]; then
	# wait up-to 1 hour for stuff to compleate downloading when network is slow
    RESULT=$( WaitLockFile $LONG_DOWNLOAD_LOCK_FILE 3600 )
fi

if [[ "$RESULT" == *"result=200"* ]]; then
	# clear lock
	test -f "$LONG_DOWNLOAD_LOCK_FILE" && rm "$LONG_DOWNLOAD_LOCK_FILE"
	RESULT=$(am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD --es "ip_address" "$ADDRESS" --es "post_data" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" --es "post_path" "/cgi-bin/updated.cgi")
	if [[ "$RESULT" == *"result=100"* ]]; then
	    # wait up-to 10 min for sending response to devman when network is slow
	    RESULT=$( WaitLockFile $LONG_DOWNLOAD_LOCK_FILE 600 )
	fi
	if [[ "$RESULT" == *"result=200"* ]]; then
		echo "YES"
	elif [[ "$RESULT" == *"result=404"* ]]; then
		echo "Failed to download."
	else
		SupervisorLog "Downloader failed (RESULT=$RESULT). It may not be installed or running properlly, or network is extremly slow."
		StopApp 'com.utils.downloader'
	fi
elif [[ "$RESULT" == *"result=404"* ]]; then
	echo "Failed to download."
else
	SupervisorLog "Downloader failed (RESULT=$RESULT). It may not be installed or running properlly, or network is extremly slow."
	StopApp 'com.utils.downloader'
fi
