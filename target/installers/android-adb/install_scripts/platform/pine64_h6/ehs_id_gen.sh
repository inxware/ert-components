#!/system/bin/sh


EhsReadId(){
  MAIN_ID="/system/etc/ehs_supervisor/.id"
  if [ -f "$MAIN_ID" ]; then
    cat "$MAIN_ID"
  fi
}

EhsOverwriteId(){
  EHS_ID=$( EhsReadId )
  echo "$EHS_ID" > "/sdcard/.EHS/.ehs"
}

EhsWifiMac(){
  cat /sys/class/net/wlan0/address
}

EhsEthMac(){
  FIRST_6="36:c9:e3"
  WIFI_MAC=$( EhsWifiMac )
  if [ -n "$WIFI_MAC" ]; then
    ETH_MAC=$( echo $WIFI_MAC | cut -c 9- )
    echo ${FIRST_6}${ETH_MAC}
  fi
}

EhsCreateMac(){
  ETH_MAC=$( EhsEthMac )
  if [ -n "$ETH_MAC" ]; then
    LOCAL_ID="/sdcard/.EHS/.ehs"
    if [ -f "$LOCAL_ID" ]; then
      rm $LOCAL_ID
    fi
    MAIN_ID="/system/etc/ehs_supervisor/.id"
    mount -o rw,remount /
    sleep 2
    rm $MAIN_ID
    echo "$ETH_MAC" > /system/etc/ehs_supervisor/.id
    sleep 1
    mount -o ro,remount /
    sleep 2
    reboot
  fi
}

EhsSetupId(){
  EHS_ID=$( EhsReadId )
  if [ -z "$EHS_ID" ]; then
    EhsCreateMac
  fi
  
  ETH_MAC=$( EhsEthMac )
  if [ "$EHS_ID" != "$ETH_MAC" ]; then
    EhsCreateMac
  fi
}
