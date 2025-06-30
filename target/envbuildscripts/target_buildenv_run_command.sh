#!/bin/bash
####################################################################################################
# This file script will create docker image for the target platform and run the arguemnt string 
# as a command and then exit.  
####################################################################################################
set -e
#set -x 

if [ "$1" = "" ]; then
    echo "You must provide at least one arguemnt for the command to run"
    exit 1
fi

SUDO_COMMAND= # Dont do this. Use the docker group for the user instead:

PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
read DOCKER_IMAGE <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
#echo "|${PATH_TO_TARGET_DOCKER_IMAGE}|"
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
    echo  "---------------------------------------------------------------------------------------------------------"
    echo "--- INFO: Running in Docker Container..."
    echo "--- If you get access errors then you probably don't belong to the docker group - please fix this with"
    echo -e "   \$sudo groupadd docker"
    echo -e "   \$sudo usermod -aG docker \$USER"
    echo -e "   \$newgrp docker"
    echo  "---------------------------------------------------------------------------------------------------------"

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
            -e EHS_ML_SUPPORT -e EHS_MV_SUPPORT -e EHS_USE_LIBCAMERA \
            -e FLASH_BOARD $DOCKER_EXTRA_ENVS"

    #echo -n "Found Docker image ${PATH_TO_TARGET_DOCKER_IMAGE} ... "
    DOCKER_STAGING_DIR="${PWD}/../TARGET_TREES/DOCKER/cachespace"
    mkdir -p  ${DOCKER_STAGING_DIR} ||exit
    pushd ${DOCKER_STAGING_DIR} || exit
    echo " Setting up ${DOCKER_IMAGE}"
#    ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} ||:
echo    ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image"
        #echo "${SUDO_COMMAND} docker run $INX_ERTCOMPONENTS_BUILDENV --user $(id -u):$(id -g) --rm --privileged -it --device=/dev/ttyACM0 -v $(pwd)/../../../:/inxware  -w /inxware/ert-components/ ${DOCKER_IMAGE} $@"

        ${SUDO_COMMAND} docker run ${INX_ERTCOMPONENTS_BUILDENV}  \
            --user $(id -u):$(id -g) --rm --privileged -it --device=/dev/ttyACM0 \
            -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"

              #These are not needed for bash initiated docker -e EHS_DEBUGALL -e DEFS
          
    else 
        echo "Trying to pull "
            echo "found Dockerfile.."
            #Get the Image Name we want to run (and possibly build)
            #todo we should  check if the imagei is already available? Or needs a force update
            echo "Attempting to pull docker pull ${DOCKER_IMAGE}"
	    ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} || echo "Could not find ${DOCKER_IMAGE} in remote repository.  use make publishddockerimage to fix this if you have a Dockerfile" 
            echo "Current PWD = "$(pwd)
            ${SUDO_COMMAND} docker run $INX_ERTCOMPONENTS_BUILDENV  \
            --user $(id -u):$(id -g) --rm --privileged -it --device=/dev/ttyACM0 \
            -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"
    fi
    popd
else
    echo "---------------------------------------------------------------------------------------------------------"
    echo "--- ERROR: No Docker Image found." 
    echo "Perhaps you want to build this on your host with make -j 8?"
    echo "---------------------------------------------------------------------------------------------------------"
    exit 1
fi
