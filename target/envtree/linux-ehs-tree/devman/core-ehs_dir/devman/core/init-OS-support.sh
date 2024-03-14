#!/bin/bash

######################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# inxware unix startup script to configure OS aspect of devman
# Dependendencies on path envifornment variables being set: 
######################################################################


##remove any lock files at start up
rm -f  "${DEVMANCOREDIR}${VARIABLE_CACHE}"dl.lock
rm -f "${DEVMANCOREDIR}${VARIABLE_CACHE}"working.flag

##remove the device add cache in case it has changed
rm -f "${DEVMANCOREDIR}${VARIABLE_CACHE}"HWID_NETIP*
#LEGACY
rm -f "${DEVMANCOREDIR}"HWID_NETIP*

## Set it to something if possible
set -- `"${DEVMANCOREDIR}"getHWID-NETIP.sh` # @todo this should use the variables
INXDEVICEID=$1
INXDEVICEIP=$2
if [ ${#INXDEVICEID} -gt 5 ] # want at least five chars for a valid ID
then
   echo -e "$INXDEVICEID\n$INXDEVICEIP\n" > "${DEVMANCOREDIR}${VARIABLE_CACHE}"HWID_NETIP
fi

#Try ans start cron if it isn't already running
if [ ! $(pidof crond) ]
then
test /usr/sbin/crond  &&  /usr/sbin/crond > /dev/null
test /bin/crond  &&  /bin/crond > /dev/null
# - this will fail harmlessly ubuntu
fi

if [ ! $(pidof cron) ]
then
test /usr/sbin/cron  &&  /usr/sbin/cron > /dev/null
test /bin/cron  &&  /bin/cron > /dev/null
fi

# Create Cron file to point at the EHS install tree
mkdir -p "${SYSDATA}${VARIABLE_CACHE}"
echo "* * * * * ${INXWAREROOT}/devman/core/sys-timer.sh > /dev/null" > "${SYSDATA}${VARIABLE_CACHE}"all.crons
cat "${SYSDATA}sys".crons     >> "${SYSDATA}${VARIABLE_CACHE}"/all.crons #pick up any other system crontab stuff
if [ -e "${SYSDATA}${VARIABLE_CACHE}"/all.crons ]
then
	#NOTE: cron truncates the file path to some arbitary amount. cd to where the file is load it to avoid this problem.
	pushd "${SYSDATA}${VARIABLE_CACHE}"
	crontab all.crons
	popd
	rm  "${SYSDATA}${VARIABLE_CACHE}"/all.crons
else
   "${SYSDATA}"default.crons # make sure we run the system update crons if nothing else - should never happen
fi

