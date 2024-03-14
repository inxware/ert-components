#!/bin/bash
# inx limited 2012
# Creates a Debian .deb package and removes duplicate libaries from EHS tree that 
# can be installed as dependencies for debian

set -e

SPECIFIC_TARGET=$1

#tdo2023 this all needs to be changed to use $EHS_DEBIAN_VERSION and $GNU_ARCH 

echo "################################## STARTING UPLOAD_EHS_DEBIAN #################################################"
#test -z "${UPLOAD}" && export UPLOAD="xxxx@devman.inx-systems.com:/home/inxware-packages/deb-repo"
if [ -z "${UPLOAD}" ]; then
	echo "You must specific a Devman enabled debian server with env varible \$UPLOAD"
	echo " e.g."
	echo "make upload ehs_deb UPLOAD=xxxx@devman.inx-systems.com:/home/inxware-packages/deb-repo"
	echo "Exiting ..."
	exit 1
fi

if [ -z "$1" ]; then
    echo "This script should be called with Make all (or the targt name is provided asthe first argument"
    exit 1
fi
EHS_VERSION_SUGGEST=`sed -n 1p ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/sysdata/version.nfo`
if [ -z "${EHS_VERSION}" ]; then
echo "No version set in  EHS_VERSION, using version.nfo file value (${EHS_VERSION_SUGGEST})."
echo "use make targetenv or make EHS_VERSION=2.x.x targetenv_deb for a one off unmaintained version"
#echo "ctrl-C to exit or continue with ${EHS_VERSION_SUGGEST}"
#read -n 1
EHS_VERSION="${EHS_VERSION_SUGGEST}"
echo "Found EHS version = ${EHS_VERSION}"
fi

echo "----------------------------------------------------------------------------------------------------------------"
echo "---  Debian package configuration for $SPECIFIC_TARGET            "
echo "---  SYSTEM_VARIANT=${SYSTEM_VARIANT}       "
echo "---  EHS_DEBIAN_VERSION=${EHS_DEBIAN_VERSION}                     "
echo "---  uploading to ${UPLOAD} **              "
echo "----------------------------------------------------------------------------------------------------------------"

#add a default ssh port
test -z "${UPLOADPORT}" && export  UPLOADPORT=8822

#All the following is now only done in targtenv_upload_deb.sh
# Check to see if this needs to be installed in a Debian package server
#test -z "${UPLOADPORT}" && export  UPLOADPORT=8822
#
echo "UPLOADING deb package (SYSTEM_VARIANT=${SYSTEM_VARIANT} to ${UPLOAD} ..."
UPLOAD_DOMAIN=` echo "${UPLOAD}" | cut -d':' -f1 `
UPLOAD_PATH=` echo "${UPLOAD}" | cut -d':' -f1 --complement`

echo "scp -P ${UPLOADPORT} ../TARGET_TREES/ehs_deb-${SPECIFIC_TARGET}/*.deb \"${UPLOAD}/\""
scp -P ${UPLOADPORT} ../TARGET_TREES/ehs_deb-${SPECIFIC_TARGET}/*.deb "${UPLOAD}/"
if [ "${EHS_DEBIAN_VERSION}" = "9" ]; then
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./install64_command.sh || echo \"Done as Debian 9- Check messages!\""
elif [ "${EHS_DEBIAN_VERSION}" = "11" ]; then
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./installdeb11_command.sh || echo \"Done as Debian 11 - Check messages!\""
elif [ "${EHS_DEBIAN_VERSION}" = "8" ]; then
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./install_command.sh || echo \"Done as Debian 8- Check messages!\""
else
#we assume this is the debian 8 repo - we probably want to change this default now??
	echo "VARIANT=${SYSTEM_VARIANT}"
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./install_command.sh || echo \"Done as Debian 8- Check messages!\""
fi
echo "################################## FINISHED UPLOAD_EHS_DEBIAN #################################################"