#!/bin/bash

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
export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}

TARGET_ENV_APK_HACKS=${EHS_ROOT}"/target/envbuildscripts/targetenv_make_apk_hacks/"${SPECIFIC_TARGET}"_hacks.sh"
if ! [ -f "$TARGET_ENV_APK_HACKS" ]; then
    echo "Cannot do syspatch upload for ANDROID ($SPECIFIC_TARGET). Hack script doesn't exist for this target."
	exit 1
fi

export TARGET_SYSPATCH=${TARGET_PATH}/syspatch

source $TARGET_ENV_APK_HACKS
echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

# setup ssh keys
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
