#!/system/bin/sh

EhsReadId(){
  settings get secure android_id | tr -d "\n"
}

EhsOverwriteId(){
  EHS_ID=$( EhsReadId )
  echo "$EHS_ID" > "$EHS_STORAGE_LOCATION/.ehs"
}

EhsSetupId(){
  # setup id
  SupervisorLog "Setting ID."
}