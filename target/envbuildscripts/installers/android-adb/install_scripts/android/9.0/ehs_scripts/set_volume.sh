#!/system/bin/sh

VALUE=$1
if [ -n "$VALUE" ]; then
    MAX_VALUE=15
    VOLUME=$((( $VALUE * $MAX_VALUE ) / 100 ))
    service call audio 7 i32 3 i32 $VOLUME i32 1 s16 "com.inx.ehs"
else
    echo "ERROR - no input arg"
fi
