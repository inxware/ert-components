#!/bin/bash
set -e

echo "Building Docker image locally"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"
DOCKER_IMAGE_NAME="${PWD}/target/platform/${TARGET}/Dockerimagename"
read DOCKER_IMAGE < ${DOCKER_IMAGE_NAME} || echo "Could not read ${DOCKER_IMAGE_NAME} "
echo $DOCKER_IMAGE
docker build -f ${PATH_TO_TARGET_DOCKERFILE} --network=host -t ${DOCKER_IMAGE} .
echo "Dockerimage build complete (not pushed to registry)"
