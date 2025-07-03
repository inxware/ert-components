#!/system/bin/sh

MAX_VALUE=15
AUDIO_SYSDUMP=$( dumpsys audio | sed -n '/- STREAM_MUSIC:/,/- STREAM_ALARM:/p' | grep "Current:" )
VOLUME_VALUE=$( echo "$AUDIO_SYSDUMP" | sed 's/.*(speaker): \(.*\),/\1/' | cut -d, -f1 )
VALUE=$((( 100 * $VOLUME_VALUE ) / $MAX_VALUE ))
echo "$VALUE"
