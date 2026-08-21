#!/bin/bash
set -e

echo "Building Docker image locally"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${TARGET}/Dockerfile"
DOCKER_IMAGE_NAME="${PWD}/target/platform/${TARGET}/Dockerimagename"
read DOCKER_IMAGE < ${DOCKER_IMAGE_NAME} || echo "Could not read ${DOCKER_IMAGE_NAME} "
echo $DOCKER_IMAGE

# Platform-specific pre-build hook.
# If the platform directory contains a build_docker_pre.sh, source it before
# building.  The hook may prompt the user interactively for credentials (the
# docker build step itself cannot prompt) and should export any env vars that
# are to be forwarded to docker build as BuildKit secrets via ERT_DOCKER_SECRETS.
#
# Format of ERT_DOCKER_SECRETS (set by the hook): space-separated list of
# BuildKit secret specs, e.g. "id=qnx_user,env=QNX_USER id=qnx_pass,env=QNX_PASS"
# These are passed to docker build as --secret <spec> ... arguments.
PLATFORM_PRE_HOOK="${PWD}/target/platform/${TARGET}/build_docker_pre.sh"
if [ -f "${PLATFORM_PRE_HOOK}" ]; then
    echo "Sourcing platform pre-build hook: ${PLATFORM_PRE_HOOK}"
    source "${PLATFORM_PRE_HOOK}"
fi

# Build --build-arg flags from ERT_DOCKER_BUILD_ARGS if set by the hook
BUILD_ARG_FLAGS=""
for kv in ${ERT_DOCKER_BUILD_ARGS}; do
    BUILD_ARG_FLAGS="${BUILD_ARG_FLAGS} --build-arg ${kv}"
done

docker build \
    -f ${PATH_TO_TARGET_DOCKERFILE} \
    --network=host \
    -t ${DOCKER_IMAGE} \
    ${BUILD_ARG_FLAGS} \
    .
echo "Dockerimage build complete (not pushed to registry)"
