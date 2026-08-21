#!/bin/bash

# creating the esp final binaries into the target/envbuildscripts/ directory along with all the others

####################################################################################################
# This file script will build the apk from make products.  
####################################################################################################

set -e

# Use a separate image name file for Unity export so that make all_docker
# (which uses Dockerimagename) continues to use the C/C++ build image while
# targetenv_unity_export_docker uses the GameCI Unity editor image.
export DOCKER_IMAGE_NAME_FILE="Dockerimagename-unity"
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/${DOCKER_IMAGE_NAME_FILE}"

if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
	read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
	echo "################################################ DOCKER Exporting Unity ##################################################################"
	echo "Building ${TARGET} using Docker image: $Dockerimagename"
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "./target/envbuildscripts/targetenv_unity_export.sh ${TARGET}"
    echo "################################################ DOCKER BUILDING APK IMAGE DONE #############################################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi  
