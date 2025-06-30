#!/system/bin/sh

MAX_VALUE=15
VOLUME_VALUE=$( am broadcast -a com.utils.downloader.broadcastreceiver.UTILS --ei get_master_volume 1 | sed -n 's/.*data="\([^"]*\)".*/\1/p' )
VALUE=$((( 100 * $VOLUME_VALUE ) / $MAX_VALUE ))
echo "$VALUE"
