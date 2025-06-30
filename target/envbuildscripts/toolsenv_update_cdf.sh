#!/bin/bash

set -e

DIST_PATH="../dist"

# check if the installer directory is present
if ! [ -d "../inxware-installer" ]; then
    echo "'inxware-installer' repository is not present. This repo must be present in order to create dist/ directry."
    echo "Would you like to clone this repository? (y/n)"
    read -n 1 WHAT
    if [ "${WHAT}" = "y" ]; then
        git clone ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/inxware-installer.git ../inxware-installer
    else
        echo "make sure 'inxware-installer' is cloned next to this repository."
        exit 1
    fi
fi

if [ -d "../inxware-installer" ] && [ -d "$DIST_PATH" ]; then
    CURRENT_PATH=$( pwd )
    cd "../inxware-installer"
    ./scripts/utils/copy_feature_profiles.sh ${DIST_PATH} ${FEATURE_PROFILES}
    cd ${CURRENT_PATH}
else
    echo "Failed to update CDFs"
fi