#!/bin/bash

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1

echo "Setting up PINE64 A6 device."

shell_push_ex ehs_service.rc /system/etc/init/ehs_service.rc
shell_exec 'chown 0.0 /etc/init/ehs_service.rc'
shell_exec 'chmod 0644 /etc/init/ehs_service.rc'
shell_exec 'chcon u:object_r:system_file:s0 /etc/init/ehs_service.rc'

# read macaddress and create id out of it
# copy mac-id generator
shell_push './ehs_id_gen.sh' '/system/etc/ehs_supervisor/ehs_id_gen.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_id_gen.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_id_gen.sh;'

# add gpio setup script
shell_push './ehs_gpio_setup.sh' '/system/etc/ehs_supervisor/ehs_gpio_setup.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_gpio_setup.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_gpio_setup.sh;'

# updated auto-routes file to fix audio-jack issue
shell_push './a64_paths.xml' '/etc/a64_paths.xml'
shell_exec 'chmod 0644 /etc/a64_paths.xml;'

# overwrite settings
shell_exec 'settings put global wifi_on 0'
shell_exec 'settings put global bluetooth_on 0'
shell_exec 'settings put global audio_safe_volume_state 2'
shell_exec 'setprop persist.sys.timezone Europe/London'
