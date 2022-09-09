#!/bin/bash

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1

echo "Installing Ambifier supervisor scripts via adb"

# overwrite app manager script
shell_push './ehs_app_manager.sh' '/system/etc/ehs_supervisor/ehs_app_manager.sh'
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_app_manager.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_app_manager.sh;'


# uninstall all apps which might have been previously installed
EHS_APP="com.inx.ehs"
AMBIFIER_APP="com.TheSoundAgency.Ambifier2"
SSO_APP="online.show.signplayer"
uninstall ${SSO_APP}
uninstall ${EHS_APP}
uninstall ${AMBIFIER_APP}
