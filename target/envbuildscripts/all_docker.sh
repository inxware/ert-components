#!/bin/bash
####################################################################################################
# This file script will build the ert source using the docker file available or if not will use 
# the host's make system.  
####################################################################################################

set -e

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.
if [[ "$TARGET" == *"esp32s3"* ]]
then
    echo "DEFS += TARGET_OS_VERSION_STRING='\"esp32s3.$(date +'%Y%m%d%H%M%S').$(git rev-parse --short HEAD).$(grep -o '".*"' ${PWD}/target/os-arch/esp32s3_freertos-xtensa/target_version_string.mk  | sed 's/"//g' | cut -d. -f4 | awk '{print $1 + 1}')\"'" > ${PWD}/target/os-arch/esp32s3_freertos-xtensa/target_version_string.mk
    pushd ${PWD}
    git add target/os-arch/esp32s3_freertos-xtensa/target_version_string.mk
    git update-index --skip-worktree target/os-arch/esp32s3_freertos-xtensa/target_data_bin.c
    popd
fi

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

