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

    #echo -n "Found Docker image ${PATH_TO_TARGET_DOCKER_IMAGE} ... "
    DOCKER_STAGING_DIR="${PWD}/../TARGET_TREES/DOCKER/cachespace"
    mkdir -p  ${DOCKER_STAGING_DIR} ||exit
    pushd ${DOCKER_STAGING_DIR} || exit
    echo " Setting up ${DOCKER_IMAGE}"
#    ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} ||:
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image "
        ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm --privileged -it \
             -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"
    else 
        echo "Trying to pull "
            echo "found Dockerfile.."
            #Get the Image Name we want to run (and possibly build)
            #todo we should  check if the imagei is already available? Or needs a force update
            echo "Attempting to pull docker pull ${DOCKER_IMAGE}"
	    ${SUDO_COMMAND}docker pull  ${DOCKER_IMAGE} || echo "Could not find  ${DOCKER_IMAGE} in remote repository.  use make publishddockerimage to fix this if you have a Dockerfile" 
            echo "Current PWD = "$(pwd)
            ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm --privileged -it \
            -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            "$@"
    fi
    popd
fi
