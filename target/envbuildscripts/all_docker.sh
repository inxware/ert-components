#!/bin/bash
####################################################################################################
# This file script will build the ert source using the docker file available or if not will use 
# the host's make system.  
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
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "pwd && ls -l .. &&  make -j 8 "
    echo "###################################################---DONE---#####################################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " Building using the host make because there is no Dockerimage specificed for this target."
    echo " Use plain make to do this if you don't like this mesage."
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

    make -j 8

    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " Built using the host make because there is no Dockerimage specificed for this target."
    echo " Use plain make to do this if you don't like this mesage :)"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

