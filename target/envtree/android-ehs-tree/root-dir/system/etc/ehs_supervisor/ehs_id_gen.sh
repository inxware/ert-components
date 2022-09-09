#!/system/bin/sh


EhsReadId(){
  cat /system/etc/ehs_supervisor/.id
}

EhsOverwriteId(){
  EHS_ID=$( EhsReadId )
  echo "$EHS_ID" > "/sdcard/.EHS/.ehs"
}

EhsSetupId(){
  echo "EHS ID Setup"
}
