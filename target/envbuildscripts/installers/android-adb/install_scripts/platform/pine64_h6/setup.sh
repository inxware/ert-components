#!/bin/bash
#TODO All these ehs_settings.sh ecript's system file pushes should be done in targetenv, like everythign else.


source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

echo "Setting up PINE64 H6 device."

# overwrite init.rc file
shell_push_ex init.rc /
shell_exec 'chown root:shell /init.rc; chmod 0750 /init.rc;'

# generate unique mac address id for the eth0 (use first 6 digits of the original address)
shell_push './ehs_eth0_mac.sh' '/system/etc/ehs_supervisor/ehs_eth0_mac.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_eth0_mac.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_eth0_mac.sh;'

# copy eth0 mac and id generator
shell_push './ehs_id_gen.sh' '/system/etc/ehs_supervisor/ehs_id_gen.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_id_gen.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_id_gen.sh;'

# copy event handler overwrite script
shell_push './ehs_settings.sh' '/system/etc/ehs_supervisor/ehs_settings.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_settings.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_settings.sh;'

# turn off the sleep mode
shell_exec 'settings put global stay_on_while_plugged_in 1'

# overwrite settings
shell_exec 'setprop persist.sys.timezone Europe/London'

# overwrite a bootloader animation
shell_push_ex './bootanimation.zip' '/system/media/bootanimation.zip'
