#!/bin/bash

source $INSTALL_DIR/install_scripts/install_utils.sh || exit 1

# overwrite server configure script
shell_push './ehs_server_configure.sh' '/system/etc/ehs_supervisor/ehs_server_configure.sh' || exit 1
write_server_config ${DEVMAN_SERVER_NAME} ${DEVMAN_SERVER_URL} || exit 1
shell_exec 'chown root:shell /system/etc/ehs_supervisor/ehs_server_configure.sh; chmod 0755 /system/etc/ehs_supervisor/ehs_server_configure.sh;' || exit 1

# install certificates
install_certs || exit 1
