#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# Sort the version info for the target & more:
# Reads the current version number for the repo and populates the target's version info file.
# If INC_VERSION  is set to yes then the version number is incremeneted (causing a complete set of rebuilds)
# and also causing a tag entry to be netered into the repository
#

source ./scripts/build-function-library/colour.sh
TXT_FG=${TXT_FG_CYAN}

########################################################################
## add version information to the tree
echo
echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
echo
echo "Starting 'TARGETENV_CREATE_VERSION_INFO'"
if [ -n "${1}" ]; then
    TARGET_BASE_DIR=../TARGET_TREES/ehs_env-$1
else
    err "No target directory name provided to create_version_info script"
    exit 1
fi

#test if we should create a new version number
if [ "${2}" == "INC_VERSION" ]; then
    INC_VERSION=yes
else
    INC_VERSION=no
fi

#We might be called without the SPECIFIC_TARGET VARIABLE SET so use the arg
if [ -z "${SPECIFIC_TARGET}" ]; then
    SPECIFIC_TARGET=$1
fi

# override path
if [ -n "${3}" ]; then
    TARGET_BASE_DIR=$3
fi

echo adding Version Information to the tree
# This is done here (rather than earlier so that the TIMESTAMP identifier matches that in the repo
# try to use the same time stamp as the caller to be coherent with repo tag
if [ -z ${TIME_STAMP} ]; then
    TIME_STAMP=`date +%F-%Hh%Mm%Ss`
fi

if [ -n "${VERSION_NAME}" ] ; then
    echo "Version Name provided: $VERSION_NAME"
    #write line 1
    echo "${VERSION_NAME}-" > $TARGET_BASE_DIR/sysdata/version.nfo
    git log -n 1| grep commit | sed 's/commit\s*\([0..9,a..f,A..F]*\)/\1/' >> $TARGET_BASE_DIR/sysdata/version.nfo
else
    # Get the current version from the repo
    echo "Getting the version information from the repo..."

    MAJORMAJOR_V=`sed -n 1p ./Releases/version_strings`
    MAJOR_V=`sed -n 2p ./Releases/version_strings`
    MINOR_V=`sed -n 3p ./Releases/version_strings`
    echo "got repo version ${TXT_FG_WHITE}${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_V}"
    if [ "${INC_VERSION}" == "yes" ]; then
        MINOR_VV=`expr $MINOR_V + 1`
        echo "Updating repo file ./Releases/version_strings, with version ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV}"
        echo ${MAJORMAJOR_V} > ./Releases/version_strings
        echo ${MAJOR_V} >> ./Releases/version_strings
        echo ${MINOR_VV} >> ./Releases/version_strings

        git add ./Releases/version_strings
        git commit -m "Checking in revision file for ${VERSION_NAME} with time stamp ${TIME_STAMP} (build=${TARGET})" ./Releases/version_strings
        git push
    else
        MINOR_VV=$MINOR_V
    fi
    #and finally write the data
    mkdir -p $TARGET_BASE_DIR/sysdata/
    echo ${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV} > $TARGET_BASE_DIR/sysdata/version.nfo
fi

echo $TIME_STAMP >>  $TARGET_BASE_DIR/sysdata/version.nfo
git log -n 1 | grep commit | sed 's/commit\s*\([0..9,a..f,A..F]*\)/\1/' >> $TARGET_BASE_DIR/sysdata/version.nfo
echo  "$SPECIFIC_TARGET" >> $TARGET_BASE_DIR/sysdata/version.nfo
echo "EHS" >> $TARGET_BASE_DIR/sysdata/version.nfo
if [ "${INC_VERSION}" == "yes" ]; then
    VERSION_NAME="${MAJORMAJOR_V}.${MAJOR_V}.${MINOR_VV}-${TARGET}"
    # Create in repo tag and give this a proper name
    # Checking in all code
    echo "Creating Tag ${TIME_STAMP}-${VERSION_NAME} in the repository"

    #git add ./Releases/version_string
    git commit -am "Adding full code for revision ${VERSION_NAME} with time stamp ${TIME_STAMP}"
    git tag -a ${VERSION_NAME} -m "Build ${VERSION_NAME} - ${TIME_STAMP}"
    # ./Releases/version_string

    #write the version info into the target tree
    echo Created Tag in repository
fi

exit 0
