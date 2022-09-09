#!/bin/bash

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1

echo "Setting up Q-BOX device."

# overwrite init.rc file
shell_push_ex init.rc /
shell_exec 'chown root:shell /init.rc; chmod 0750 /init.rc;'

# read macaddress and create id out of it
MACADDRESS=$( shell_exec 'cat /sys/class/net/eth0/address | tr -d "\n"' )
echo "Creating new mac address ($MACADDRESS) file."
shell_exec 'echo '${MACADDRESS}' > "/system/etc/ehs_supervisor/.id"'

# copy event handler overwrite script
shell_push './ehs_settings.sh' '/system/etc/ehs_supervisor/ehs_settings.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_settings.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_settings.sh;'

# This is from H6. Are there any settings in Q-Box that needs to be overwritten?
# turn off the sleep mode
#shell_exec 'settings put global stay_on_while_plugged_in 1'

# overwrite settings
#shell_exec 'setprop persist.sys.timezone Europe/London'

# overwrite a bootloader animation
#shell_push_ex './bootanimation.zip' '/system/media/bootanimation.zip'
