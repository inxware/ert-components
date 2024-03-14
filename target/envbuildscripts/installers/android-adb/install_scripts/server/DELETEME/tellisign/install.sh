#!/bin/bash

source ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/install_scripts/install_utils.sh || exit 1

# overwrite server configure script
shell_push './ehs_server_configure.sh' '/system/etc/ehs_supervisor/ehs_server_configure.sh' || exit 1
write_server_config ${DEVMAN_SERVER_NAME} ${DEVMAN_SERVER_URL} || exit 1
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_server_configure.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_server_configure.sh;' || exit 1
# This is so they don't have to configure each device via the UI to get the right time.
# todo2023 - we neeed this to use a variable and not the hardwired values wehn we refactor.
if [ "$NETWORK_NTP_SERVER" != "" ];then
	shell_exec 'settings put global ntp_server $NETWORK_NTP_SERVER'
fi
#echo "Setting up the host for domain ($DEVMAN_SERVER_DOMAIN)"
#adnoc devman - can't rely on adnox DNS so we put this in hosts.
#Warning - we only want to do this once!
if [ "$NETWORK_HARDWIRED_HOSTS" != "" ];then 
	shell_exec 'echo -e  "\n${NETWORK_HARDWIRED_HOSTS}\n" >> /system/etc/hosts' || exit 1
fi
# install certificates
install_certs || exit 1
