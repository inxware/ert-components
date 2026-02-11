#!/bin/bash
#TODO All these ehs_settings.sh ecript's system file pushes should be done in targetenv, like everythign else.


source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

echo "Setting up firefly stationpc M2 device."

# setup the startup script
shell_push_ex ehs_service.rc /system/vendor/etc/init/ehs_service.rc
shell_exec 'chown 0.0 /system/vendor/etc/init/ehs_service.rc'
shell_exec 'chmod 0644 /system/vendor/etc/init/ehs_service.rc'
shell_exec 'chcon u:object_r:system_file:s0 /system/vendor/etc/init/ehs_service.rc'

# read macaddress and create id out of it
# copy mac-id generator
shell_push './ehs_id_gen.sh' '/system/etc/ehs_supervisor/ehs_id_gen.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_id_gen.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_id_gen.sh;'
# copy event handler overwrite script
shell_push './ehs_settings.sh' '/system/etc/ehs_supervisor/ehs_settings.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_settings.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_settings.sh;'

# overwrite settings
# disable wifi/bluetooth ? 
# #shell_exec 'settings put global wifi_on 0'
# #shell_exec 'settings put global bluetooth_on 0'
shell_exec 'settings put global audio_safe_volume_state 2'
shell_exec 'setprop persist.sys.timezone Europe/London'
