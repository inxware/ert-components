
####################################################################################################
# This file script will run everything that is needed before building traget (in docker).
####################################################################################################

set -e

# Check to see if we should use a docker image
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.
echo ${PATH_TO_TARGET_DOCKER_IMAGE}
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
    read Dockerimagename <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "
    echo "############################################ RUNNING ESP32S3 TARGETENV IN DOCKER ##########################################"
    echo "Building ${TARGET} using Docker image: $Dockerimagename"
    echo "------------------------------------------------------------------------------------------------------------------------"
    ./target/envbuildscripts/target_buildenv_run_command.sh sh -c "./target/envbuildscripts/targetenv_pre_build.sh ${TARGET}"
    echo "############################################ COMPLETED ESP32S3 TARGETENV IN DOCKER ########################################"
else
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi