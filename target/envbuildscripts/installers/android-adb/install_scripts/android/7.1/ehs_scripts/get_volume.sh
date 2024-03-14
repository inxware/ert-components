#!/system/bin/sh

if [ "$1" == "default" ]; then # check if it's in default format
    IS_DEFAULT_FORMAT="yes"
fi
AUDIO_SYSDUMP=$( dumpsys audio )
CURRENT_DEVICE_SYSDUMP=$( echo "$AUDIO_SYSDUMP" |  grep "mMainType=" | sed 's/.*mMainType=\(.*\)/\1/' | cut -d, -f1 )
CURRENT_VOLUME_SYSDUMP=$( echo "$AUDIO_SYSDUMP" | sed -n '/- STREAM_MUSIC:/,/- STREAM_ALARM:/p' | grep "Current:" )
DEVICE_NAME="speaker"
if [ "$CURRENT_DEVICE_SYSDUMP" == "0x0" ]; then # speaker
    DEVICE_NAME="speaker"
elif [ "$CURRENT_DEVICE_SYSDUMP" == "0x1" ] || [ "$CURRENT_DEVICE_SYSDUMP" == "0x9" ]; then # headset
    DEVICE_NAME="headset"
elif [ "$CURRENT_DEVICE_SYSDUMP" == "0x2" ] || [ "$CURRENT_DEVICE_SYSDUMP" == "0xa" ]; then # headphones
    DEVICE_NAME="headphone"
elif [ "$CURRENT_DEVICE_SYSDUMP" == "0x8" ]; then # hdmi
    DEVICE_NAME="hdmi"
fi
VOLUME_VALUE=$( echo "$CURRENT_VOLUME_SYSDUMP" | sed 's/.*('${DEVICE_NAME}'): \(.*\),/\1/' | cut -d, -f1 | awk '{print $1;}')
if [ -n "$IS_DEFAULT_FORMAT" ]; then
    echo $VOLUME_VALUE
else
    MAX_VALUE=15
    VALUE=$((( 100 * $VOLUME_VALUE ) / $MAX_VALUE ))
    echo "$VALUE"
fi
