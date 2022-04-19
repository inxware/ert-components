#!/bin/bash
######################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# This file is run by cron to run the OS aspect of devman and any
# other periodic tasks.
# Needs to be run as root.
# This will block new events for up to ${LOCKTIMEOUT} if previous events 
# block too long, hang, or don't set the done flag
######################################################################

export PATH=$PATH:/sbin/:/bin/:/usr/bin/

# We get the EHS root by reference of the path used to call this script (from cron)
COMLINE=$0
COMLINECHAR1=${COMLINE:(0):(1)}
if [ ${COMLINECHAR1} = "/" ]; then
   export INXWAREDEVMANROOT=`dirname $0`/
else
   export INXWAREDEVMANROOT=$PWD/`dirname $0`/
fi

export LOCKTIMEOUT=120
# 120 minutes and we'll allow new events to be processed

# Note INXWAREDEVMANROOT is exported for the update script to use.

DEVMANCOREDIR="../../devman/core/"
DEVMANPLUGINDIR="../../devman/plugins/"
VARIABLE_CACHE="var/"

if [ -e ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}dl.lock ]; then
    if [  ! -z $(find "${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}dl.lock" -cmin "-$LOCKTIMEOUT")  ]; then
	# remove the lock flag allow try next time.
#todo	killall -q update.sh
#todo	killall -q --older-than 10m sys-timer.sh # kill any zombies, but not this typically.
    	rm  ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}dl.lock
	exit # let's wait another minut before trying again.
    else
	exit # do nothing this time still doing last instruction
    fi
else #OK 
	## Do the mission critical ping and download - neve to be updated with syspatches!
	mkdir -p  ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE} && touch  ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}dl.lock
	${INXWAREDEVMANROOT}${DEVMANCOREDIR}update.sh
	rm  ${INXWAREDEVMANROOT}${DEVMANCOREDIR}${VARIABLE_CACHE}dl.lock
fi 
