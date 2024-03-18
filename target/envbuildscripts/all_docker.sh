#!/bin/bash
####################################################################################################
# This file script will build the ert source using the docker file available or if not will use 
# the host's make system.  
####################################################################################################

set -e

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"

if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
	read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
	echo "################################################# DOCKER COMPILATION ######################################################################"
	echo "Building ${TARGET} using Docker image: $Dockerimagename"
	echo "###########################################################################################################################################"
 	./target/envbuildscripts/target_buildenv_run_command.sh sh -c "pwd && ls -l .. &&  make -j 8 "
    echo "########################## DOCKER COMPILATION DONE WITH IMAGE $Dockerimagename ###############################################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " Building using the host make because there is no Dockerimage specificed for this target."
    echo " Use plain make to do this if you don't like this mesage."
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

    #We need to run this from a clean normal login shell environment, because we have already run make , which polutes the environment
    env -i bash -l -c make -j 8

    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " Built using the host make because there is no Dockerimage specificed for this target."
    echo " Use plain make to do this if you don't like this mesage :)"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

