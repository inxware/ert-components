#!/bin/bash

# inx limited 2020
# Uploads android supervisor bundle and APKs to the server

set -e

export SPECIFIC_TARGET=$1
export EHS_ROOT=`pwd` # assuming we're in the ehs project root

pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd

echo ""
echo "***** SYSPATCH UPLOADER FOR ANDROID ($SPECIFIC_TARGET) *****"
echo ""

if [ -z "$SPECIFIC_TARGET" ]; then
    echo "TARGET is not specified."
    exit 1
fi
pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory is not available!"
    exit 1
fi

# uses this to export env vars
source ${EHS_ROOT}/target/envbuildscripts/targetenv_android_dep_pack.sh $SPECIFIC_TARGET "no-run"

# This looks for  a servers ssh public key to see if we need to create one or not
SshKeyFile(){
    if [ -n "$DEVMAN_SERVER_DOMAIN" ] && [ -n "$DEVMAN_UNAME" ]; then
        KEY_NAME=$(echo "$DEVMAN_SERVER_DOMAIN" | sed -r 's/[.]+/_/g')
        KEY_NAME=${DEVMAN_UNAME}"_"${KEY_NAME}
        SSH_FILE="$HOME/.ssh/$KEY_NAME"
        echo $SSH_FILE
    fi
}

# Generate an ssh key for a particular server and upload it to a server
# Presumable a password prompt will be needed when this runs
SetupSshKey(){
    # generate ssh key
    SSH_FILE=$1
    if [ -n "$SSH_FILE" ]; then
        echo "Settup SSH key for Devman Server URL"
        if test -f "$SSH_FILE"; then
            echo "key $SSH_FILE exists"
        else
            echo "ssh Key ($SSH_FILE) doesn't exist"
            echo "creating SSH key pair for the devman server"
            ssh-keygen -N "" -f $SSH_FILE
        fi
        echo "Add key to ($DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN)"
        ssh-copy-id -i ${SSH_FILE} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    fi

}


# Magical function that knows alot about things that are likely to not be true.
# It will put android packages in places Devman may be able to deploy them to devices.
# todo2023 - we now have scripts built into Devman servers that know how to put patches and patches in the 
# right place and we should use this method instead as soon as possible.

# todo2023 - the following should copy the static scripts from the repo to a staging directory and then
# add the newly built apk as the dldata.tgz

# The static scripts are now found in here
# envbuildscripts/installers/android-adb/devman/os-patch
# The apks should be removed from the EHS repo. (UNless the supervisor one can't be rebuilt easily. In which case we can move it to contrib-middleware perhaps?)
# This function Uploads things to Devman from ../TARGET_TREES/..../apps/ (Possibly put there by targetenv or apk_pack)
# These are then deployed via OS Updated method)
# No Idea what the ci version is and what the -debug one is really?
UploadDevmanAppUpdatesData(){
    SSHKEY=$1
    echo "Uploading apps syspatch data for ($SPECIFIC_TARGET)"
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/apps
    EHS_UPDATES_SCRIPT=${TARGET_SYSPATCH_APPS}/dldata.sh
    EHS_UPDATES_DATA=${TARGET_SYSPATCH_APPS}/dldata.tgz
    SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"
    
    if [[ $SPECIFIC_TARGET == *-ci ]]; then
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}-ci/"
    elif [[ $SPECIFIC_TARGET == *-debug ]]; then
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}-debug/"
    else
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}/"
    fi
    if [ "$DEVMANUID" != "" ]; then
        DEVMAN_UNAME="$DEVMANUID"
        echo "Change devman username to $DEVMAN_UNAME"
    fi
    echo "SSHKEY ======> $SSHKEY"
    echo "ADDRESS =====> $DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    echo "LOCATION ====> $EHS_UPDATES_SERVER_DIR"
    ssh ${SSHKEY} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN" 'mkdir -p "'${EHS_UPDATES_SERVER_DIR}'"' || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_DATA} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_SCRIPT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
}
#Uploads Supervisor update scripts to the right place on the Devman server (for OS Updated method) 
UploadDevmanSupervisorUpdatesData(){
    SSHKEY=$1
    echo "Uploading supervisor syspatch data for ($SPECIFIC_TARGET)"
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/supervisor
    EHS_UPDATES_SCRIPT=${TARGET_SYSPATCH_APPS}/dldata.sh
    EHS_UPDATES_DATA=${TARGET_SYSPATCH_APPS}/dldata.tgz
    SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"
    EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-supervisor-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}/"
    if [ "$DEVMANUID" != "" ]; then
        DEVMAN_UNAME="$DEVMANUID"
        echo "Change devman username to $DEVMAN_UNAME"
    fi
    echo "SSHKEY ======> $SSHKEY"
    echo "ADDRESS =====> $DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    echo "LOCATION ====> $EHS_UPDATES_SERVER_DIR"
    ssh ${SSHKEY} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN" 'mkdir -p "'${EHS_UPDATES_SERVER_DIR}'"' || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_DATA} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_SCRIPT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
}

echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

# setup ssh keys
# The following seems to cfeate new ones all the ime anyway as far I can tell
IGNORE_SSH_KEY=yes 
SSHKEY=
if [ -z "$IGNORE_SSH_KEY" ]; then
    SSH_FILE=$( SshKeyFile )
    if [ -z "$SSH_FILE" ]; then
        echo "SSH key is not specified"
    else
        SetupSshKey $SSH_FILE
        SSHKEY="-i $SSH_FILE"
    fi
fi
UploadDevmanSupervisorUpdatesData "$SSHKEY"
UploadDevmanAppUpdatesData "$SSHKEY"
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
