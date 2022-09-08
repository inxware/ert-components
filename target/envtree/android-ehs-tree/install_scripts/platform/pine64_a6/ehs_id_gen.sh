#!/system/bin/sh

EhsReadId(){
  cat /sys/class/net/eth0/address | tr -d "\n"
}

EhsOverwriteId(){
  EHS_ID=$( EhsReadId )
  echo "$EHS_ID" > "$EHS_STORAGE_LOCATION/.ehs"
}

EhsSetupId(){
  # setup id
  SupervisorLog "Setting ID."
}