#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# Runs any OS update scripts on first run. 
# 
# Supported Agruments: none - expects INXWAREROOT to be set
########################################################################
set +e

test -n "$INXWAREROOT"  || exit

pushd $INXWAREROOT/bin/HostOsInit || exit


export PATH=${PATH}:/sbin:/bin:/usr/bin:/user/sbin

test -f /opt/devboard && exit
#Only run once
test -f 0000-done && exit

#try to avoid parallel execution from ropey daemonise scripts
test -f 0000-doing && exit

touch 0000-doing
for i in `ls -v *.sh`; do 
	source $i || :
done

touch 0000-done
rm 0000-doing
#Only reboot on updates - not suring an initial install as this upsets the OS installer, which will reboot afterwards anyway

if [ -f /opt/ehs/userdata/0000-not-first-boot ]; then
	reboot
else
	touch /opt/ehs/userdata/0000-not-first-boot
fi

