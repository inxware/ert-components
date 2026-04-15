#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# This file script will create docker image for the target platform and run the arguemnt string
# as a command and then exit.

set -e
#set -x

if [ "$1" = "" ]; then
    echo "You must provide at least one arguemnt for the command to run"
    exit 1
fi

SUDO_COMMAND= # Dont do this. Use the docker group for the user instead

# DOCKER_IMAGE_NAME_FILE can be overridden by the caller (e.g. targetenv_unity_export_docker.sh
# sets it to "Dockerimagename-unity" so the GameCI image is used instead of the C/C++ build image).
DOCKER_IMAGE_FILENAME="${DOCKER_IMAGE_NAME_FILE:-Dockerimagename}"
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/${DOCKER_IMAGE_FILENAME}"
read DOCKER_IMAGE < "${PATH_TO_TARGET_DOCKER_IMAGE}" || { err "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE}"; exit 1; }

#echo "|${PATH_TO_TARGET_DOCKER_IMAGE}|"
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
    echo  "--------------------------------------------------------------------"
    echo "--- INFO: Running in Docker Container..."
    echo "--- If you get access errors then you probably don't belong to the docker group - please fix this with"
    echo -e "   \$sudo groupadd docker"
    echo -e "   \$sudo usermod -aG docker \$USER"
    echo -e "   \$newgrp docker"
    echo  "--------------------------------------------------------------------"

    DOCKER_EXTRA_ENVS=""
    KEYWORD="EHS_TARGETENV_PREBUILD_"
    # Use compgen to list all shell variables and filter by keyword pattern
    # No idea how to use this...??? What is compgen??? Do we do something with PREBUILD??? I hope we don't need this really!
    while IFS= read -r name; do
        if [ "$name" != "" ]; then
            DOCKER_EXTRA_ENVS="$DOCKER_EXTRA_ENVS -e $name"
        fi
    done < <(compgen -A variable | grep "$KEYWORD")

    INX_ERTCOMPONENTS_BUILDENV="-e EHS_OS -e  EHS_ARCH -e  EHS_GNU_OS -e EHS_GNU_ARCH -e SYSTEM_VARIANT -e SPECIFIC_TARGET -e DEVMAN_SERVER_NAME \
        -e DEVMAN_SERVER_DOMAIN -e DEVMAN_SERVER_PROTOCOL -e TARGET -e EHS_PRODUCT_NAME -e TARGET_PATH -e TARGET_SYSPATCH -e EHS_HOST_DEBIAN_BUILD\
        -e EHS_DEBIAN_VERSION -e INXWARE_TARGETENV_HACKS -e ANDROID_STUDIO_JNILIBS_PATH -e EXE -e EHS_ANDROID_PACKAGE_SIGNING_PATH -e EHS_DEBIAN_VERSION \
        -e EHS_UNITY_PROJECT_EXPORT_SUPPORT -e DEBIAN_PACKAGE_NAME -e EHS_PLUGIN_LIBRARY_DEPENDENCY -e EHS_GUI_SUPPORT -e DEBIAN_PACKAGE_PLATFORM_EXTRA \
        -e DEBIAN_PACKAGE_EXTRA -e EHS_AUTO_START -e DEVMAN_SERVER_DOMAIN_1 -e DEVMAN_SERVER_DOMAIN_2 -e EHS_DEVMAN_SUPERVISOR_REQUIRED \
        -e EHS_ML_SUPPORT -e EHS_MV_SUPPORT -e EHS_USE_LIBCAMERA -e TOOLCHAIN_NAME -e COMPONENT_BASE_TECHNOLOGIES -e FLASH_BOARD -e ANDROID_STUDIO_EHS_PROJECT \
        -e FLASH_BOARD -e TEST_FUNC -e ERT_INIT -e EHS_NO_LIBXML2_SUPPORT -e EHS_EXCLUDE_XML_PARSER -e EHS_SKIP_GNULIBRARIES \
        -e UNITY_LICENSE $DOCKER_EXTRA_ENVS"
        
    #echo -n "Found Docker image ${PATH_TO_TARGET_DOCKER_IMAGE} ... "
    DOCKER_STAGING_DIR="${PWD}/../TARGET_TREES/DOCKER/cachespace"
    mkdir -p  ${DOCKER_STAGING_DIR} || { err ""; exit 1; }
    pushd ${DOCKER_STAGING_DIR} || { err ""; exit 1; }

    echo " Setting up ${DOCKER_IMAGE}"
    #${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} ||:

    echo "${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}"
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image"
        
        ${SUDO_COMMAND} docker run ${INX_ERTCOMPONENTS_BUILDENV}  \
            --user $(id -u):$(id -g) --rm --privileged -i $([ -t 0 ] && echo "-t") --device=/dev/ttyACM0 \
            -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"

        #These are not needed for bash initiated docker -e EHS_DEBUGALL -e DEFS
    else
        #echo "Trying to pull "
        #echo "found Dockerfile.."
        #Get the Image Name we want to run (and possibly build)
        #todo we should  check if the imagei is already available? Or needs a force update

        echo "Attempting to pull docker pull ${DOCKER_IMAGE}"
        ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} || echo "Could not find ${DOCKER_IMAGE} in remote repository. Use 'make publish_docker_image' to fix this if you have a Dockerfile"

        echo "Current PWD = $(pwd)"
        ${SUDO_COMMAND} docker run $INX_ERTCOMPONENTS_BUILDENV  \
            --user $(id -u):$(id -g) --rm --privileged -i $([ -t 0 ] && echo "-t") --device=/dev/ttyACM0 \
            -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"
    fi
    popd
else
    echo "--------------------------------------------------------------------"
    echo "--- ERROR: No Docker Image found."
    echo "Perhaps you want to build this on your host with make -j 8?"
    echo "--------------------------------------------------------------------"
    exit 1
fi
