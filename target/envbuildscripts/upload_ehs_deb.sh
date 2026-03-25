#!/bin/bash
# inx limited 2012
# Creates a Debian .deb package and removes duplicate libaries from EHS tree that 
# can be installed as dependencies for debian

set -e

if [ -z "$1" ]; then
    echo "This script should be called with Make all (or the targt name is provided asthe first argument"
    exit 1
fi

SPECIFIC_TARGET=$1

echo "################################## STARTING UPLOAD_EHS_DEBIAN #################################################"
if [ -z "${UPLOAD}" ]; then
    echo "Using your configuration from arget/devman-config/*.mk to identify the server"

    #Check we have enough information in the target/devman-config/*.mk if we don't have an override
    if [ "$DEVMAN_SERVER_DOMAIN" = "" -a "$DEBIAN_INXWARE_SERVER_DOMAIN" = "" ]; then 
        echo "ERROR: You must set DEVMAN_SERVER_DOMAIN or DEBIAN_INXWARE_SERVER_DOMAIN in your seleted target/devman/configs/<server name>.mk file"
        return 1
    fi
    if [ "$DEVMAN_SERVER_PROTOCOL" = "" ]; then 
        echo "ERROR: You must set DEVMAN_SERVER_PROTOCOL to http or https in your seleted target/devman/configs/<server name>.mk file"
        return 1
    fi
    if [ "$DEVMAN_UNAME" = "" ]; then 
        echo "ERROR: You must set DEVMAN_SERVER_DOMAIN in your seleted target/devman/configs/<server name>.mk file"
        return 1
    fi
    #todo 2024 do we really need the EHS_PRODUCT_NAME? We are using it because the deb repo server may use this for its path, 
    # but this will trip us up one day because we don't really know what EHS_PRODUCT_NAME name is used for conceptually? 
    # We should change this to EHS_PRODUCT_TYPE_NAME which woul be useful for grouping all specific platform targets that 
    # are essentially the same app on different targets for example
    if [ "$EHS_PRODUCT_NAME" = "" ]; then
        echo "WARNING: $EHS_PRODUCT_NAME is not set. Setting to inxware-packages "
        export EHS_PRODUCT_NAME="inxware-packages"
    fi
    if [ "$SSHPORT" = "" ]; then
        echo "WARNING: SSHPORT is not set - using port 22"
        export SSHPORT=22
    fi

    #we sometimes provide the server on another URL (currently assumes same username as Devman server host)
    if [ "$DEBIAN_INXWARE_SERVER_DOMAIN" = "" ]; then
        #ssh format: xxxx@devman.inx-systems.com:/home/inxware-packages/deb-repo
        UPLOAD="${DEVMAN_UNAME}@${DEVMAN_SERVER_DOMAIN}:/home/${EHS_PRODUCT_NAME}/deb-repo"
        UPLOAD_DOMAIN="${DEVMAN_UNAME}@${DEVMAN_SERVER_DOMAIN}"
    else
        UPLOAD_DOMAIN="${DEVMAN_UNAME}@${DEBIAN_INXWARE_SERVER_DOMAIN}"
        UPLOAD="${DEVMAN_UNAME}@${DEBIAN_INXWARE_SERVER_DOMAIN}:/home/${EHS_PRODUCT_NAME}/deb-repo"
    fi
    UPLOAD_PATH="/home/$EHS_PRODUCT_NAME/deb-repo"
    UPLOADPORT=$SSHPORT
    
else 
    echo "Using the override credentials and path provided (${UPLOAD})"
    echo "This should be of the ssh format xxxx@devman.inx-systems.com:/home/inxware-packages/deb-repo"
    
    if [ "$SSHPORT" = "" ]; then
        echo "WARNING: SSHPORT is not set - using port 22"
        export SSHPORT=22
    fi
    UPLOAD_DOMAIN=` echo "${UPLOAD}" | cut -d':' -f1 `
    UPLOAD_PATH=` echo "${UPLOAD}" | cut -d':' -f1 --complement`
    #add a default ssh port
    test -z "${UPLOADPORT}" && export  UPLOADPORT=22
fi

UPLOADPORT=$SSHPORT
    
#Get some version ifno for the package name

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
echo "UPLOADING deb package (SYSTEM_VARIANT=${SYSTEM_VARIANT} to ${UPLOAD} ..."
echo "----------------------------------------------------------------------------------------------------------------"
echo "---  Debian package configuration for $SPECIFIC_TARGET            "
echo "---  SYSTEM_VARIANT=${SYSTEM_VARIANT}       "
echo "---  EHS_DEBIAN_VERSION=${EHS_DEBIAN_VERSION}                     "
echo "---  uploading to ${UPLOAD} **              "
echo "----------------------------------------------------------------------------------------------------------------"

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
