#!/bin/bash

source $ANDROID_VERSION_DIR/install_utils.sh

echo "Setting up T95max device."

# overwrite init.rc file
$ADB push init.rc /
$ADB shell 'chown root:shell /init.rc; chmod 0750 /init.rc;'

# generate unique macaddress and create id out of it
MACADDRESS=$( macgen ) # @TODO replace it with existing if unique
EXISTS=$( $ADB shell 'if [ -f "/system/etc/ehs_supervisor/.id" ]; then echo "YES"; fi' )
if [ -z "$EXISTS" ]; then
	echo "Creating new mac address ($MACADDRESS) file."
	$ADB shell 'echo '${MACADDRESS}' > "/system/etc/ehs_supervisor/.id"'
fi

# overwrite settings
