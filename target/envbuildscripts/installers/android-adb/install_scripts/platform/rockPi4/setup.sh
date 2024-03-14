#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

echo "Setting up RockPi4 device."

# overwrite init.rc file
shell_push_ex init.rc /
shell_exec 'chown root:shell /init.rc; chmod 0750 /init.rc;'

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

