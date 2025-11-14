#!/bin/bash
# This script updates only the devman scripts and utilities, not core ehs and uploads this to the inx devman server
# New version information is created for the runtime using the same rules as targetenv
# 

export SPECIFIC_TARGET=$1

########################################################################
# Check if we are uploading for android platform
if [[ "$SPECIFIC_TARGET" == *"android"* ]]; then
	
	
	#create a device update file for the devman server with devman scripts.
	if [ -z $DEVMAN_INTERMEDIATE_SERVER ] ; then
		DEVMAN_INTERMEDIATE_SERVER="devport.inx-systems.net"
	fi
	
	# Do some sanity checks
	if [ -z $TARGET ] ;then
		echo "TARGET is not defined - Please run configure --help and try again."
	fi
	#create default server login names
	if [ -z $DEVMAN_INTERMEDIATE_UNAME ] ; then
		DEVMAN_INTERMEDIATE_UNAME="no-user-set"
	fi
	if [[ -n ${DEVMAN_INTERMEDIATE_SSHPORT} ]]; then 
		export SSHPORT=${DEVMAN_INTERMEDIATE_SSHPORT}
	else
		export SSHPORT=22
	fi
	
	TARGET_ENV_NAME="ehs_env-$TARGET"
	#Create the path variables
	
	export STAGING_DIRECTORY="${PWD}/../TARGET_TREES/${TARGET_ENV_NAME}"
	
	if [ ! -e ../ert-components] ;then
		echo this script must be run from the ert-components directory.
		exit
	fi
	 
	TARGET_PATH=$STAGING_DIRECTORY
	if [ -d $TARGET_PATH ]; then
	    echo "Staging Directory $TARGET_PATH found - OK..."
	else
	   echo "Staging Directory $TARGET_PATH  NOT found - Please run make targetenv. EXITING!"
	   exit 1
	fi
	
	#absolute  path to relevant TRAGETTREE directory
	export TARGET_SYSPATCH=${TARGET_PATH}/syspatch
	
	if [ -d "$TARGET_SYSPATCH" ]; then
	    echo "Staging syspatch Directory $TARGET_SYSPATCH found - OK..."
    else
   		echo "Staging syspatch Directory $TARGET_SYSPATCH NOT found - Please run make targetenv. EXITING!!!"
   		exit 1s
	fi 
	
	#This is the intermeidate server path where the patches will be uploaded from the build system and transffered to the final server
	SERVER_SERVER_UPDATES_DIR="/home/inx-devman/bin/dldata-scripts/server-updates"
	SERVER_SERVER_UPDATE_LATEST_ANDROID="adnoc-0100-add-update-to-latest-player-android"   
	
	#This is the final server'spath where the device patches will be installed
	SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"
	
	#staging directory sources
	TARGET_SYSPATCH_SUPERVISOR=${TARGET_SYSPATCH}/supervisor
    SUPERVISOR_UPDATES_SCRIPT=${TARGET_SYSPATCH_SUPERVISOR}/dldata.sh
    SUPERVISOR_UPDATES_DATA=${TARGET_SYSPATCH_SUPERVISOR}/dldata.tgz
    SUPERVISOR_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-supervisor-$SPECIFIC_TARGET/"
    
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/apps
    EHS_UPDATES_SCRIPT=${TARGET_SYSPATCH_APPS}/dldata.sh
    EHS_UPDATES_DATA=${TARGET_SYSPATCH_APPS}/dldata.tgz
    EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-$SPECIFIC_TARGET/"
	
	# Make the final server tarbal that will be tarred into / 
	pushd $STAGING_DIRECTORY/
	mkdir -p "server2server/"$SERVER_SERVER_UPDATES_DIR
	pushd "./server2server"
	#make a single package of both suprvisor and EHS package
	

    echo "SUPERVISOR SERVER INSTALL STAGING LOCATION ====> ${PWD}/./$SUPERVISOR_UPDATES_SERVER_DIR"
    mkdir -p "./"${SUPERVISOR_UPDATES_SERVER_DIR} || exit 1
    cp  ${SUPERVISOR_UPDATES_DATA}   "./"${SUPERVISOR_UPDATES_SERVER_DIR} || exit 1
    cp  ${SUPERVISOR_UPDATES_SCRIPT} "./"${SUPERVISOR_UPDATES_SERVER_DIR} || exit 1
	
	# AND NOW THE EHS Package:
	echo "EHS SERVER INSTALL STAGING LOCATION ====> ${PWD}/./$EHS_UPDATES_SERVER_DIR"
    mkdir -p "./"${EHS_UPDATES_SERVER_DIR} || exit 1
    cp ${EHS_UPDATES_DATA}   "./"$EHS_UPDATES_SERVER_DIR || exit 1
    cp ${EHS_UPDATES_SCRIPT} "./"$EHS_UPDATES_SERVER_DIR || exit 1	
    # make a tarball with the full absolute path for the target server in it (Note: Only works for "canonical" server installs. 
    # We might want to consier a safer version of this that will only update to /home/inx-devman/bin/dldata-scripts/server-updates in the future
    tar -czf ./dldata.tgz ./home
    rm -r ./home
    
    
#needed for dpkg to work correctly

#echo "must set our working directory to where the script is located"

    
    
    echo "#!/bin/bash" > dldata.sh
    echo "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" >> dldata.sh
    echo 'cd "$(dirname "$0")"' >> dldata.sh
    echo "tar -xf ./dldata.tgz -C /" >> dldata.sh 
    
    #Send up the package to the host server
    echo "ADDRESS =====> $DEVMAN_INTERMEDIATE_UNAME@$DEVMAN_INTERMEDIATE_SERVER"
    echo "LOCATION ====> ${PWD}/./$EHS_UPDATES_SERVER_DIR"
    ssh -p ${SSHPORT} "$DEVMAN_INTERMEDIATE_UNAME@$DEVMAN_INTERMEDIATE_SERVER" 'mkdir -p "'${SERVER_SERVER_UPDATES_DIR}/${SERVER_SERVER_UPDATE_LATEST_ANDROID}'"' || exit 1
    scp -P ${SSHPORT} dldata.sh "${DEVMAN_INTERMEDIATE_UNAME}@${DEVMAN_INTERMEDIATE_SERVER}:${SERVER_SERVER_UPDATES_DIR}/${SERVER_SERVER_UPDATE_LATEST_ANDROID}" || exit 1
    scp -P ${SSHPORT} dldata.tgz "${DEVMAN_INTERMEDIATE_UNAME}@${DEVMAN_INTERMEDIATE_SERVER}:${SERVER_SERVER_UPDATES_DIR}/${SERVER_SERVER_UPDATE_LATEST_ANDROID}" || exit 1	
	exit 0
fi
########################################################################
echo " Sorry non-android platforms are not currently suppoerted for server2server updates"
exit



