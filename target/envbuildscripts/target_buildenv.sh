#!/bin/bash
SUDO_COMMAND= # Dot do this use the docker group instead:
echo "$PWD"
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"  
read DOCKER_IMAGE <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
echo "|${PATH_TO_TARGET_DOCKER_IMAGE}|"
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then

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
    echo " Setting up ${DOCKER_IMAGE}"
#    ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} ||:
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image "
        echo "Current PWD = "$(pwd)
        ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm --privileged -it \
             -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            sh -c "/bin/bash"
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
            sh -c "/bin/bash"
    fi
    popd
fi
