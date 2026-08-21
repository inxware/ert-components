#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# Configures OS file systems for ambifier products. 
# This file must sourced from run_ehs.sh for debian linux targets
# 
########################################################################

######################################################################
# Get a known network address is there is one, 
# so we can check the network is up before starting
######################################################################

if [ "${DEBUGMODE}" != "DONT_WAITFORNETWORK" ]; then
	TESTURL=""
	if [ -e "${DEVMANCOREDIR}/config/DEVMANURL.000" ]; then 
		TESTURL_TRY=`cat "${DEVMANCOREDIR}/config/DEVMANURL.000"`
		if [ ${#TESTURL_TRY} -gt 8 ]; then
			TESTURL="$( echo $TESTURL_TRY | sed -re 's#^http://|https://##; s#/score/$##' )"
		else
			TESTURL=""
		fi	
	fi
	if [ -n "${TESTURL}" ];then
		MAXWAIT=60
		WAITTIME=0
		PINGPERIOD=2
		ping -c 1 -w 1 ${TESTURL} &> /dev/null
		while [ $? != 0 ] && [ $WAITTIME -le $MAXWAIT ] ; do
			WAITTIME=$(($WAITTIME + ${PINGPERIOD}))
			sleep ${PINGPERIOD}
			ping -c 1 -w 1 ${TESTURL} &> /dev/null
		done
	#else we don't wait - assume there's no network if one isn't specced in DEVMANURL.000
	fi
fi # dont wait for network
#######################################################################
# Check to see if we need to run some OS configuration on first install
######################################################################
./runOsInit.sh  || :

########################################################################
# We must have wget installed for Devman: fall back install here 
########################################################################

test `which wget` || apt-get install -y wget

########################################################################
# Ambifier-Specific Initialisation
########################################################################
# Optimise for flash HDDs and try to avoid lockups on Jessie on some hardware
echo noop > /sys/block/sda/queue/scheduler

# mount the media directory - assuming this is configured in fstab
test -d /mnt/media && mount /mnt/media

# kill syslog daemon as this contends with HDD bandwidth on some very slow machines.
if [ "$4" = "KILLLOGDS" ]; then
	pidof syslogd && killall syslogd
	pidof klogd && killall klogd
fi

#start X in case it isn't
# Note the Xorg/X process name may differ between Debian 9 and 11
export DISPLAY=:0.0
if ! pidof Xorg && ! pidof X > /dev/null
then
 X &
fi

sleep 5
if ! pidof X && ! pidof Xorg> /dev/null
then
 export DISPLAY=:1
 Xvfb :1 -screen 1 1024x768x16 &
 sleep 2
 X &
fi

#GST exports - if we have some non-standard support
if [ 1 = 0 ]; then
 #export GST_DEBUG="*,2"
 if [ -e "${PWD}/cslib/gstreamer-0.10" ]; then 
   export GST_PLUGIN_PATH="${PWD}/cslib/gstreamer-0.10"
 else
   test -e "/usr/lib/gstreamer-0.10" && export GST_PLUGIN_PATH="/usr/lib/gstreamer-0.10"
 fi
fi

# work around to make ld-linux executable, should be done by installer
test -e "${PWD}/corelib/ld-linux.so.2" && chmod +x "${PWD}/corelib/ld-linux.so.2"

# work around to get devmanmon to report to first server when start ehs at boot, otherwise will fallback to report to default server
if [ -f Ambifier2_LinuxServer/Ambifier2_LinuxServer.x86_64 ]; then
sleep 2
  pidof Ambifier2_LinuxServer.x86_64 && killall Ambifier2_LinuxServer.x86_64
  pushd  Ambifier2_LinuxServer
  chmod +x Ambifier2_LinuxServer.x86_64
  ./Ambifier2_LinuxServer.x86_64 -batchmode "headless" -nographics &
#  ./Ambifier2_LinuxServer.x86_64 &
  popd

fi

#migration of fixed playlist to multi-playlist support, so previous laylist keep on playing
pushd ../userdata/media/scheds/
test -e current.smil && mv current.smil "`cat ../../configs/devman-player/MusicPlaylistName.cfg`"
popd
# we need to sleep to eait for the network to come up - or else we get lots of boot playlist check errors.
sleep 10

# start EHS 
rm "${INXWAREROOT}/sysdata/stop_all.flag"

#This is probably not needed for new OSs, but may be required to support very old devices in the field
if [ "${LIBMODE}" = "LIB_HOST" ]; then
    mkdir -p "${PWD}/cslib/disabled" ; test -f ${PWD}/cslib/libasound.so.2 && mv ${PWD}/cslib/libasound* ${PWD}/cslib/disabled/
else
    test -f "${PWD}/cslib/disabled/libasound.so.2"  && mv -f ${PWD}/cslib/disabled/libasound* ${PWD}/cslib/
fi
#####################################################################################################################################

