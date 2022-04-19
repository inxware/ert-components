#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# Configures OS file systems, Starts EHS using the appropriate OS loader, 
# sets EHS environment variables and starts OS aspect of Devman.
# 
# Supported Agruments: (Use "" as placeholders).
# 1 : NO_RESTART - stops EHS being restarted on exit 
# 2 : LIB_HOST	 - Allows EHS to use Host's runtime libraries
# 3 : GDB        - starts EHS with GDB for debugging
########################################################################

########################################################################
# Setup variable environment space
########################################################################

export RUNMODE=$1
export LIBMODE=$2
export DEBUGMODE=$3

if [ "${RUNMODE}" != "SLAVE"  -a "${DEBUGMODE}" != "SPAWNED" ];then
pidof -x -o %PPID run_ehs.sh && echo "EHS IS ALREADY RUNNING - NOT STARTING" && exit
fi

#canonical relative paths
VARIABLE_CACHE="var/"
DEVICE_CONFIG="config/"

# get the EHS  working directory 
COMLINE="${0}"
COMLINECHAR1="${COMLINE:(0):(1)}"
if [ "${COMLINECHAR1}" = "/" ]; then
  DIRECTORY="`dirname "${0}"`"
else
  DIRECTORY="${PWD}/`dirname "${0}"`"
fi
export INXWAREROOT="${DIRECTORY}/../"
export DEVMANCOREDIR="${INXWAREROOT}devman/core/"
export SYSDATA="${INXWAREROOT}sysdata/"
echo "INFO: EHS_START: Set inxware directory to ${INXWAREROOT}" 

# Optimise for flash HDDs and try to avoid lockups on Jessie on some hardware
echo noop > /sys/block/sda/queue/scheduler

######################################################################
#echeck to see if we need to daemonize 
######################################################################
if [ "${DEBUGMODE}" == "DAEMONIZE" ];then
    $0 $RUNMODE $LIBMODE SPAWNED 1> /dev/null 2> /dev/null &
    exit 0
fi

# run from known place to make $PWD work
cd "${DIRECTORY}"

######################################################################
# Get a known network address is there is one, 
# so we can check the netowrk is up before starting
######################################################################

if [ "${DEBUGMOD}" != "DONT_WAITFORNETWORK" ]; then

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

fi # donwait for network
#######################################################################
#Check to see if w need to run some OS configuration on first install
######################################################################
./runOsInit.sh  || :

#######################################################################
# start OS aspect of devman
#######################################################################
if [ -e  "${DEVMANCOREDIR}/init-OS-support.sh" ] ; then
  source "${DEVMANCOREDIR}/init-OS-support.sh"
fi

########################################################################
# We must have wget installed for Devman: fall back install here 
########################################################################

test `which wget` || apt-get install -y wget

########################################################################
# Prepare the ehs runtime 
########################################################################

# mount the media directory - assuming this is configured in fstab
test -d /mnt/media && mount /mnt/media

# kill syslog daemon as this contends with HDD bandwidth on some very slow machines.
if [ "$4" == "KILLLOGDS" ]; then
	pidof syslogd && killall syslogd
	pidof klogd && killall klogd
fi

#start X in case it isn't
# Note the Xorg/X process name may differ between Debian 9 and 11
export DISPLAY=:0.0
if ! pidof Xorg > /dev/null
then
 X &
fi

sleep 5
if ! pidof Xorg > /dev/null
then
 export DISPLAY=:1
 Xvfb :1 -screen 1 1024x768x16 &
 sleep 2
 X &
fi

# temp - this is to be moved inside the EHS_LUA toolit code.
export LUA_PATH="${INXWAREROOT}/bin/csdir/lua/scripts/?.lua;;"
export LUA_CPATH="${INXWAREROOT}/bin/csdir/lua/lib/?.so;;"

#GST exports - if we have some non-standard support
if [ 1 == 0 ]; then
#export GST_DEBUG="*,2"
if [ -e "${PWD}/cslib/gstreamer-0.10" ]; then 
  export GST_PLUGIN_PATH="${PWD}/cslib/gstreamer-0.10"
else
  test -e "/usr/lib/gstreamer-0.10" && export GST_PLUGIN_PATH="/usr/lib/gstreamer-0.10"
fi
fi

# work around to make ld-linux executable, should be done by installer
test -e "${PWD}/corelib/ld-linux.so.2" && chmod +x "${PWD}/corelib/ld-linux.so.2"

#The following delay is needed before unity is started because Debain 11/Rockchip boards don't init sound properly until a timeout.
sleep 62 
if [  1 == 1 ]; then
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
fi
#migration of fixed playlist to multi-playlist support, so previous laylist keep on playing
pushd ../userdata/media/scheds/
test -e current.smil && mv current.smil "`cat ../../configs/devman-player/MusicPlaylistName.cfg`"
popd
# we need to sleep to eait for the network to come up - or else we get lots of boot playlist check errors.
sleep 10

# start EHS 
rm "${INXWAREROOT}/sysdata/stop_all.flag"

while [ 1 ]  
do
	if [ "${LIBMODE}" == "LIB_HOST" ]
	then
	    # if we're using host libs we generally want to use ths host's alsa libs too
	    mkdir -p "${PWD}/cslib/disabled" ; test -f ${PWD}/cslib/libasound.so.2 && mv ${PWD}/cslib/libasound* ${PWD}/cslib/disabled/
	    export LD_LIBRARY_PATH="${PWD}/cslib"
	    if [ "${DEBUGMODE}" == "GDB" ]; then
		gdb    ./ehs.exe
	    else
	        ./ehs.exe || echo "Exiting ehs.exe"
	    fi
	else # run without host's userspace 	 
	    test -f "${PWD}/cslib/disabled/libasound.so.2"  && mv -f ${PWD}/cslib/disabled/libasound* ${PWD}/cslib/
	    export LD_LOADER_OVERRIDE_PATH="${PWD}/corelib/ld-linux.so.2" # This is used in inx builds of glibc to stop using host's hard-wired loader
	    export LD_LIBRARY_PATH="${PWD}/corelib:${PWD}/cslib"            # Put core and component libraries in library path
	    ./corelib/ld-linux.so.2 --library-path "${PWD}/corelib:${PWD}/cslib" ./ehs.exe # load with our loader, not the systems.
	fi
	echo "ehs: exiting!" > /dev/kmsg
	if [ "${RUNMODE}" != "YES_RESTART" ];then
		echo "Not restarting ehs.exe"
	    break
	fi
	if [ -e "${SYSDATA}stop_all.flag" ];then
	    echo "Stopping ehs"
	    break
	fi
	echo "ehs: restarting!" > /dev/kmsg
done 

