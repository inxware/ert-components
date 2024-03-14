#!/bin/bash
# This script updates only the devman scripts and utilities, not core ehs and uploads this to the inx devman server
# New version information is created for the runtime using the same rules as targetenv
# 

set -e

export SPECIFIC_TARGET=$1

########################################################################
# Check if we are uploading for android platform
if [[ "$SPECIFIC_TARGET" == *"android"* ]]; then
    echo "Building all the Android packages" 
	./target/envbuildscripts/ehs_android_syspatch_upload.sh $SPECIFIC_TARGET
	exit 0
fi
########################################################################

#create a device update file for the devman server with devman scripts.
if [ -z $DEVMANSERVER ] ; then
	DEVMANSERVER="devman.inx-systems.com"
fi
# Do some sanity checks
if [ -z $TARGET ] ;then
	echo "TARGET is not defined - Please run configure --help and try again."
fi
#create default server login names
if [ -z $DEVMANUID ] ; then
DEVMANUID="root"
fi

TARGET_ENV_NAME="ehs_env-$TARGET"
#Create the path variables

export STAGING_DIRECTORY="../TARGET_TREES/$TARGET_ENV_NAME"

if [ ! -e ../ert-components ] ;then
	echo this script must be run from the ert-components directory.
	exit
fi

# Always do this except exclusions with KEEP-XXXX env variables: 
if [ "$CLEAN_CONFIG" != "no" ]; then 
  	echo "Cleaning ALL configuration data. Leaving application data"
	./target/envbuildscripts/targetenv_clean_config.sh ${TARGET}
else # do a light clean of dynamic variables and flags only.
	export KEEP_DEVMANCONFIG=yes
	export KEEP_APPLICATION=yes
	export KEEP_USERCONFIG=yes
	./target/envbuildscripts/targetenv_clean_config.sh ${TARGET}
fi

########################################################################
##do some sanity checks
if [ -e  "$STAGING_DIRECTORY/devman/core/config/DEVMANURL.000" ];then
	echo "DANGER!!!!!!!!!!!!! You are creating a build with devman configurations!!"
	echo "Downloading this to a preconfigured target may change the DEVMAN config and leave the device lost"
	echo "Press y to continue anyway:"
if [  "$CHECKWITHAHUMAN" == "yes" ]; then 
	read -n 1 WHAT
	if [ $WHAT != "y" ] ; then 
		echo "Re-run after cleaning tree by running:"
		echo "make [XXXXX] targetenv_cleancfg"
		echo "Exiting "
		exit
	fi
fi
fi

# Optional - Update the version information 
# - Don;t this always if we are using for build testing. 
# We use a bash script for each product line that will do a make target_env_version call for tagged releases.
# ./target/envbuildscripts/targetenv_create_version_info.sh $TARGET #INC_VERSION

########################################################################
# Create a human readable tag in the repo and name the upload file with this included
#
## TIME_STAMP was created with =`date +%F-%Hh%Mm%Ss` 
export TIME_STAMP=`date +%F-%Hh%Mm%Ss`
export GIVEN_NAME="unknown"
export PLATFORM="known-platform"

if [ -e $STAGING_DIRECTORY/sysdata/version.nfo ] 
then
	# The build time stamp is stored in the env tree version information file
	GIVEN_NAME=`sed -n 1p $STAGING_DIRECTORY/sysdata/version.nfo`
	TIME_STAMP=`sed -n 2p $STAGING_DIRECTORY/sysdata/version.nfo`
	PLATFORM=`sed -n 4p $STAGING_DIRECTORY/sysdata/version.nfo`
else
	echo "ERROR: Cannot find $STAGING_DIRECTORY/sysdata/version.nfo - Aborting - You must run make [XXXX] targetenv target first"
	exit
fi

#create the patch name and directories
#UPLOAD_TIME_STAMP=`date +%H%M%S` 
# make unique name to avoid tromping
export PATCH_TYPE_DIR="system-patches"
if [ -n "$EHS_ONLY" ]; then
# this is just the ehs.exe
export PATCH_NAME="EHS-ONLY-${GIVEN_NAME}-${PLATFORM}-${TIME_STAMP}"
#PATCH_NAME="EHS-ONLY-${TIME_STAMP}-${GIVEN_NAME}-${PLATFORM}-${UPLOAD_TIME_STAMP}"
else
	if [ -z "${PATCH_NAME}" ]; then
		#this has the whole targetenv dir structure
		export PATCH_NAME="EHS-SYS-${GIVEN_NAME}-${PLATFORM}-${TIME_STAMP}"
		#PATCH_NAME="EHS-SYS-${TIME_STAMP}-${GIVEN_NAME}-${PLATFORM}-${UPLOAD_TIME_STAMP}"
	fi
fi
export ARCHIVE_DIRECTORY="./Releases/${TARGET_ENV_NAME}/${PATCH_NAME}"    
# /$PATCH_TYPE_DIR/DevMan"
export SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"

########################################################################
## OK lets do something - dldata tarballs an script
			
echo "Creating the tarball $PATCH_NAME: archiving and uploading to $DEVMANSERVER"
# tar up to include the ehs directory
cd $STAGING_DIRECTORY/
if [ -n "$EHS_ONLY" ]; then #We will do the version info and app data if it hasn't been cleaned
	tar -czf "./dldata.tgz" ./bin/ehs.exe ./sysdata/version.nfo ./appdata
else 
	tar --exclude='dldata.tgz' -czf "./dldata.tgz" *     
fi

#create the target install script - we need to cater for legacy here hence don't use reboot script in /root/ehs/bin.
echo "export PATH=$PATH:/sbin/:/bin/:/usr/bin" > "./dldata.sh"
echo "mkdir -p /opt/ehs" >> "./dldata.sh"
echo "tar --no-same-permissions --no-same-owner -C /opt/ehs -xzf dldata.tgz" >> "./dldata.sh"
#@todo - we are installing in /opt/ehs but some legacy devices are running in /root/ehs
if [ "$SYSPATCH_NEED_REBOOT" == "no" ]; then
	echo "/opt/ehs/bin/restart.sh"			 >> "./dldata.sh"
else
#	we have set the PATH so hopefully the following will work on old devices
	echo "/opt/ehs/bin/reboot.sh"			 >> "./dldata.sh"
fi

######################################################################## 

########################################################################
#update local archive
mkdir -p "../../EHS/$ARCHIVE_DIRECTORY"
cp dldata.tgz "../../EHS/$ARCHIVE_DIRECTORY/"
cp dldata.sh  "../../EHS/$ARCHIVE_DIRECTORY/"

########################################################################
##Update to server

if [[ -n ${SSHPORT} ]]; then 
	export PORT=${SSHPORT}
else
	export PORT=22
fi
#ssh $DEVMANUID@"$DEVMANSERVER" 'rm -Rf '\"${SERVER_SYSPATCH_DIR}\"'/*'
ssh -p "${PORT}" "${DEVMANUID}@${DEVMANSERVER}" 'mkdir '\"${SERVER_SYSPATCH_DIR}\"'/'\"${PATCH_NAME}\" || exit 1
scp -P "${PORT}" "./dldata.sh"  "$DEVMANUID@${DEVMANSERVER}:"\"${SERVER_SYSPATCH_DIR}\"'/'\"${PATCH_NAME}\"'/' || exit 1
scp -P "${PORT}" "./dldata.tgz" "$DEVMANUID@${DEVMANSERVER}:"\"${SERVER_SYSPATCH_DIR}\"'/'\"${PATCH_NAME}\"'/' || exit 1
echo "Complete - Installed on devman server: ${DEVMANSERVER}"
echo "PATCH NAME=${PATCH_NAME}"
########################################################################

