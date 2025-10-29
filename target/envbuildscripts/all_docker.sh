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
# This file script will build the ert source using the docker file available or if not will use 
# the host's make system.  

set -e

source ./scripts/build-function-library/colour.sh

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"

if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
    read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
    echo
    echo "--------------------------------------------------------------------"
    echo
    heading "Building ${TXT_FG_BRIGHT_GREEN}${TARGET}${TXT_FG_BRIGHT_WHITE} using Docker image: ${TXT_FG_BRIGHT_GREEN}$Dockerimagename"
    echo
    echo "--------------------------------------------------------------------"
    echo
    ./target/envbuildscripts/target_buildenv_run_command.sh sh -c "pwd && ls -l .. &&  make -j 8 "
    echo
    echo "--------------------------------------------------------------------"
    echo
    echo "Docker compilation completed using image ${TXT_FG_WHITE}$Dockerimagename"
    echo
else
    echo
    echo "--------------------------------------------------------------------"
    echo
    warn "Building using the host make because there is no Dockerimage specificed for this target."
    warn "Use plain ${TXT_FG_WHITE}make${TXT_FG_YELLOW} to do this if you don't want to see this message."
    echo
    echo "--------------------------------------------------------------------"
    echo

    #We need to run this from a clean normal login shell environment, because we have already run make , which polutes the environment
    env -i bash -l -c make -j 8

    echo
    echo "--------------------------------------------------------------------"
    echo
    warn "Built using the host make because there is no Dockerimage specificed for this target."
    warn "Expected to find the image at ${TXT_FG_WHITE}'${PATH_TO_TARGET_DOCKER_IMAGE}'"
    warn "Use plain ${TXT_FG_WHITE}make${TXT_FG_YELLOW} to do this if you don't want to see this message."
    echo
fi
