#!/bin/bash

#TODO2025 THIS FILE SHOULD BE REMOVED FROM THE COMMUNITY VERSION.

set -e

DIST_PATH="../dist"

# check if the installer directory is present
if ! [ -d "../inxware-installer" ]; then
    echo "'inxware-installer' repository is not present. This repo must be present in order to create dist/ directry."
    echo "Would you like to clone this repository? (y/n)"
    read -n 1 WHAT
    if [ "${WHAT}" = "y" ]; then
        #TODO2025 - this needs to use a url from repo_config.mk a mentioned elsewhere for the build system
        git clone --depth 1 ssh://git@github.com:inxware/inxware-installer.git ../inxware-installer
    else
        echo "make sure 'inxware-installer' is cloned next to this repository."
        exit 1
    fi
fi

mkdir -p ${DIST_PATH}
if [ -d "../inxware-installer" ] && [ -d "$DIST_PATH" ]; then
    CURRENT_PATH=$( pwd )
    cd "../inxware-installer"
    ./scripts/utils/copy_feature_profiles.sh ${DIST_PATH} ${FEATURE_PROFILES}
    cd ${CURRENT_PATH}
else
    echo "Failed to update CDFs. ./inxware-installer/ directoy does not exist"
fi
