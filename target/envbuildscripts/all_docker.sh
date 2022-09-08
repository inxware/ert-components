#!/bin/bash
# Check to see if we should use a docker image
#SUDO_COMMAND=sudo # tdo make this default off
SUDO_COMMAND= # Dot do this use the docker group instead:
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"  
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.
echo "|${PATH_TO_TARGET_DOCKER_IMAGE}|"
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
    #if newgrp docker ; then
    #echo Switched to docker group OK
    #else
    echo "Running in Docker Container..."
    echo "If you get access errors then you probably don't belong to the docker group - please fix this with"
    echo -e "  \$sudo groupadd docker"
    echo -e "  \$sudo usermod -aG docker \$USER"
    echo -e "  \$newgrp docker"
    #fi
    echo -n "Found Docker image... :"
    DOCKER_STAGING_DIR="${PWD}/../TARGET_TREES/DOCKER/cachespace"
    mkdir -p  ${DOCKER_STAGING_DIR} ||exit
    pushd ${DOCKER_STAGING_DIR} || exit
    read DOCKER_IMAGE <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "  
    echo " Setting up ${DOCKER_IMAGE}"
#    ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} ||:
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image "
        echo "Current PWD = "$(pwd)
        ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm --privileged -it \
             -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            sh -c "pwd && ls -l && make -j 8"
        #sh -c "pwd && ls -al && ls -al ../"
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
            sh -c "pwd && ls -l .. &&  make -j 8 "
    fi
    popd
    
else
    make
fi

