#!/bin/bash

####################################################################################################
# This file script will build the windows installer exe from make products.  
####################################################################################################

set -e

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.

if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
	read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
	echo "################################################ DOCKER BUILDING WINDOWS INSTALLER FOR EHS  ############################################################"
	echo "Building ${TARGET} using Docker image: $Dockerimagename"
	echo "########################################################################################################################################################"
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "./target/envbuildscripts/targetenv_make_nsis.sh ${TARGET} ${ERT_PACKAGE_NAME} ${ERT_NSIS_EXE_NAME}"
    echo "################################################### DOCKER BUILDING WINDOWS INSTALLER FOR EHS DONE #####################################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi  
