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
fi # donwait for network
#######################################################################
#Check to see if w need to run some OS configuration on first install
######################################################################
./runOsInit.sh  || :

########################################################################
# We must have wget installed for Devman: fall back install here 
########################################################################

test `which wget` || apt-get install -y wget

#######################################################################
# start OS aspect of devman
#######################################################################
if [ -e  "${DEVMANCOREDIR}/init-OS-support.sh" ] ; then
  source "${DEVMANCOREDIR}/init-OS-support.sh"
fi

########################################################################
# Prepare the ehs runtime 
########################################################################

if test -f ${PWD}/platform-specific.sh
then
	source ${PWD}/platform-specific.sh
fi

while [ 1 ]  
do
	if [ "${LIBMODE}" == "LIB_HOST" ]
	then
	    # if we're using host libs we generally want to use ths host's alsa libs too
	    export LD_LIBRARY_PATH="${PWD}/cslib"
	    if [ "${DEBUGMODE}" == "GDB" ]; then
		gdb    ./ehs.exe
	    else
	        ./ehs.exe || echo "Exiting ehs.exe"
	    fi
	else # run without host's userspace 	 
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

