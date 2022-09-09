#!/system/bin/sh

ADDRESS=$1 
ID=$2
OUT_DIR=$3

LONG_DOWNLOAD_LOCK_FILE="/sdcard/.EHS/.longDownload"

EhsWaitDownloadFile(){
	D_RETRIES=1000 # 1000 seconds
	D_DELAY=1 # [sec]
	D_COUNTER=0
    D_RESULT="None"
	while [ "$D_COUNTER" -lt "$D_RETRIES" ]; do
		if [ -f "$LONG_DOWNLOAD_LOCK_FILE" ]; then
			D_RESULT=$( cat $LONG_DOWNLOAD_LOCK_FILE )
            rm $LONG_DOWNLOAD_LOCK_FILE
			break
		fi
		D_COUNTER=$((COUNTER+1))
		sleep $D_DELAY
	done
    echo $D_RESULT
}

if [ -f "$LONG_DOWNLOAD_LOCK_FILE" ]; then
	rm $LONG_DOWNLOAD_LOCK_FILE
fi
RESULT=$(am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD --es "ip_address" "$ADDRESS" \
 --es "check_available" /cgi-bin/isupdated.cgi --es "available_response" "data_ready" --es "available_post" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" \
 --es "download_path0" "/DeviceGroup/DEFAULT/DEFAULT/$ID/dldata.sh" --es "output_path0" "$OUT_DIR/dldata.sh" --es "ignore_failed0" "No" \
 --es "download_path1" "/DeviceGroup/DEFAULT/DEFAULT/$ID/dldata.tgz" --es "output_path1" "$OUT_DIR/dldata.tgz" --es "ignore_failed1" "Yes" \
 --es "pre_download_post" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" --es "post_path" "/cgi-bin/downloading.cgi")
 
if [[ "$RESULT" == *"result=608"* ]]; then
    RESULT=$( EhsWaitDownloadFile )
fi

if [[ "$RESULT" == *"result=200"* ]]; then
	if [ -f "$LONG_DOWNLOAD_LOCK_FILE" ]; then
		rm $LONG_DOWNLOAD_LOCK_FILE
	fi
	RESULT=$(am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD --es "ip_address" "$ADDRESS" --es "post_data" "Group=DEFAULT&DeviceType=DEFAULT&DeviceID=$ID" --es "post_path" "/cgi-bin/updated.cgi")
	if [[ "$RESULT" == *"result=608"* ]]; then
	    RESULT=$( EhsWaitDownloadFile )
	fi
	if [[ "$RESULT" == *"result=200"* ]]; then
		echo "YES"
	elif [[ "$RESULT" == *"result=404"* ]]; then
		echo "Failed to download."
	else
		echo "Downloader may not be installed or running properlly."
		am force-stop 'com.utils.downloader'
	fi
elif [[ "$RESULT" == *"result=404"* ]]; then
	echo "Failed to download."
else
	echo "Downloader may not be installed or running properlly."
	am force-stop 'com.utils.downloader'
fi
