#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

echo "Initalise scripts for Android 11.0"
# overwrite utils
shell_push './ehs_utils.sh' '/system/etc/ehs_supervisor/ehs_utils.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_utils.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_utils.sh;'


# overwrite scripts
shell_exec 'if [ -d "/system/etc/ehs_scripts" ]; then rm -rf /system/etc/ehs_scripts; fi'
shell_push './ehs_scripts' '/system/etc/'
shell_exec 'chown root:shell /system/etc/ehs_scripts/*; chmod 0755 /system/etc/ehs_scripts/*;'
