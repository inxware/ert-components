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
	echo "##################################################################################################################"
	echo "Building ${TARGET} using Docker image: $Dockerimagename"
	echo "##################################################################################################################"
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "./target/envbuildscripts/targetenv_make_apk.sh ${TARGET}"
    echo "###################################################---DONE---#####################################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi  
