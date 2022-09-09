#!/bin/bash
echo "Building Docker image"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"
DOCKER_IMAGE_NAME="${PWD}/target/platform/${TARGET}/Dockerimagename"  
read DOCKER_IMAGE <  ${DOCKER_IMAGE_NAME} || echo "Could not read ${DOCKER_IMAGE_NAME} "
echo $DOCKER_IMAGE
#echo "$DOCKER_IMAGE_NAME"
docker build -f ${PATH_TO_TARGET_DOCKERFILE} -t  ${DOCKER_IMAGE}  .
echo "Dockerimage build complete"
#echo "Please enter username"
#read $USERNAME
#echo "Please enter password"
#read $PASSWORD
USERNAME=
PASSWORD=
echo "login to Dockerimage"
docker login --username "$USERNAME" --password "$PASSWORD"
echo "pushing Dockerimage"
docker push $DOCKER_IMAGE
echo "Dockerimage Pushed"
