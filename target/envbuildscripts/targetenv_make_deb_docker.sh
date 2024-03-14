#!/bin/bash

# creating the esp final binaries into the target/envbuildscripts/ directory along with all the others

####################################################################################################
# This file script will build the apk from make products.  
####################################################################################################

set -e

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.

if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
	read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
	echo "############################################ RUNNING TARGETENV_MAKE_DEB IN DOCKER ##########################################"
	echo "Building ${TARGET} using Docker image: $Dockerimagename"
	echo "---------------------------------------------------------------------------------------------------------------------------"
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "./target/envbuildscripts/targetenv_make_deb.sh ${TARGET}"
    echo "########################################## COMPLETED TARGETENV_MAKE_DEB IN DOCKER ##########################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi  
