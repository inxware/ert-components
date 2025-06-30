#!/system/bin/sh

VALUE=$1
if [ -n "$VALUE" ]; then
    MAX_VALUE=15
    VOLUME=$((( $VALUE * $MAX_VALUE ) / 100 ))
    am broadcast -a com.utils.downloader.broadcastreceiver.UTILS --ei set_master_volume $VOLUME
else
    echo "ERROR - no input arg"
fi
