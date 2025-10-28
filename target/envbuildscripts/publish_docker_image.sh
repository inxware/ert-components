#!/bin/bash
set -e

echo "Building Docker image"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"
DOCKER_IMAGE_NAME="${PWD}/target/platform/${TARGET}/Dockerimagename"  
read DOCKER_IMAGE <  ${DOCKER_IMAGE_NAME} || echo "Could not read ${DOCKER_IMAGE_NAME} "
echo $DOCKER_IMAGE
#echo "$DOCKER_IMAGE_NAME"
docker build -f ${PATH_TO_TARGET_DOCKERFILE} --network=host -t  ${DOCKER_IMAGE}  .
echo "Dockerimage build complete"
USERNAME=
PASSWORD=
printf "%s" "Please enter username: "
read -r USERNAME
printf "%s" "Please enter password: "
read -rs PASSWORD
echo "login to Dockerimage"
docker login --username "$USERNAME" --password "$PASSWORD"
echo "pushing Dockerimage"
docker push $DOCKER_IMAGE
echo "Dockerimage Pushed"
