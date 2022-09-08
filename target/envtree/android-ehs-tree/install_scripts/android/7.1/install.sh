#!/bin/bash

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1

echo "Initalise scripts for Android 7.1"
# overwrite utils
shell_push './ehs_utils.sh' '/system/etc/ehs_supervisor/ehs_utils.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_utils.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_utils.sh;'


# overwrite scripts
shell_exec 'if [ -d "/system/etc/ehs_scripts" ]; then rm -rf /system/etc/ehs_scripts; fi'
shell_push './ehs_scripts' '/system/etc/'
shell_exec 'chown root:shell /system/etc/ehs_scripts/*; chmod 0755 /system/etc/ehs_scripts/*;'
shell_exec '/system/bin/sh /etc/ehs_scripts/set_volume.sh 70'
