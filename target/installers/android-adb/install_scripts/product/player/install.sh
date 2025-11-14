#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

echo "Installing EHS Player application"

# overwrite app manager script
shell_push './ehs_app_manager.sh' '/system/etc/ehs_supervisor/ehs_app_manager.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_app_manager.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_app_manager.sh;'

# uninstall all apps which might have been previously installed
EHS_APP="com.inx.ehs"
UNITY_APP="com.inx.appx"
SSO_APP="online.show.signplayer"
uninstall ${SSO_APP}
uninstall ${EHS_APP}
uninstall ${UNITY_APP}
