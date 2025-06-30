#!/bin/bash

set -e

INST_TARGET_NAME=$1
INST_ASSETS_ONLY=${ASSETS_ONLY}

if [ -z "${EHS_APPLAND_INST_OS_NAME}" ]; then
    EHS_APPLAND_INST_OS_NAME="${EHS_OS}-${EHS_ARCH}"
fi
if [ -z "${EHS_APPLAND_INST_DEPLOY_NAME}" ]; then
    EHS_APPLAND_INST_DEPLOY_NAME=${INST_TARGET_NAME}
fi
INST_OS_NAME=${EHS_APPLAND_INST_OS_NAME}
INST_DEPLOY_FILE_NAME=${EHS_APPLAND_INST_DEPLOY_NAME}
INST_SERVER_UNAME="root"
INST_SERVER_URL="devport.inx-systems.com"
INST_SERVER_PATH=/home/www/devport/docs/downloads/appland/runtimes/${INST_OS_NAME}

EHS_PLATFORM_APPLAND=./target/platform/${INST_TARGET_NAME}/appland
EHS_TARGET_TREES=../TARGET_TREES
EHS_TARGET_APPLAND=${EHS_TARGET_TREES}/ehs_appland_upload/${INST_TARGET_NAME}
EHS_TARGET_DEB=${EHS_TARGET_TREES}/ehs_deb-${INST_TARGET_NAME}
EHS_TARGET_NSIS=${EHS_TARGET_TREES}/ehs_nsis-${INST_TARGET_NAME}
EHS_TARGET_IMG=${EHS_TARGET_TREES}/ehs_env-${INST_TARGET_NAME}/bin/ehs.img
EHS_TARGET_APK=${EHS_TARGET_TREES}/ehs_env-${INST_TARGET_NAME}/bin/ehs.apk
EHS_TARGET_UF2=${EHS_TARGET_TREES}/ehs_env-${INST_TARGET_NAME}/bin/ehs.uf2
EHS_TARGET_ZIP=${EHS_TARGET_TREES}/ehs_env-${INST_TARGET_NAME}/bin/eRT_Package.zip

echo "###################### Uploading to Appstore ############################"
echo "Server     : ${INST_SERVER_UNAME}@${INST_SERVER_URL}"
echo "Target     : ${INST_TARGET_NAME}"
echo "OS         : ${INST_OS_NAME}"
echo "#########################################################################"

if ! [ "${EHS_APPLAND_INST_SUPPORT}" = "yes" ]; then
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo "Appland upload is not supported for this target. To support it,"
    echo "make sure to set 'EHS_APPLAND_INST_SUPPORT=yes' in config.mk"
    echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    exit 1
fi

# clear appland staging directory
test -d ${EHS_TARGET_APPLAND} && rm -rf ${EHS_TARGET_APPLAND}
mkdir -p ${EHS_TARGET_APPLAND}

if [ "${INST_ASSETS_ONLY}" != "yes" ]; then

# copy installer
if ls ${EHS_TARGET_DEB}/*.deb 1> /dev/null 2>&1; then # check if .deb file exists
    LATEST_DEB=$(ls -t ${EHS_TARGET_DEB}/*.deb | head -n 1)
    echo "copy ${LATEST_DEB} ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.deb"
    cp ${LATEST_DEB} ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.deb
elif [ -f "${EHS_TARGET_NSIS}/inxware-ert-installer.exe" ]; then
    echo "copy ${EHS_TARGET_NSIS}/inxware-ert-installer.exe ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.exe"
    cp ${EHS_TARGET_NSIS}/inxware-ert-installer.exe ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.exe
elif [ -f "${EHS_TARGET_IMG}" ]; then
    echo "copy ${EHS_TARGET_IMG} ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.img"
    cp ${EHS_TARGET_IMG} ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.img
elif [ -f "${EHS_TARGET_APK}" ]; then
    echo "copy ${EHS_TARGET_APK} ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.apk"
    cp ${EHS_TARGET_APK} ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.apk
elif [ -f "${EHS_TARGET_UF2}" ]; then
    echo "copy ${EHS_TARGET_UF2} ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.uf2"
    cp ${EHS_TARGET_UF2} ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.uf2
elif [ -f "${EHS_TARGET_ZIP}" ]; then
    echo "copy ${EHS_TARGET_ZIP} ===> ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.zip"
    cp ${EHS_TARGET_ZIP} ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.zip
else
    echo "Failed to find traget specific installer."
    exit 1
fi

else
    echo "Uploading appland assets only ..."
fi

# copy appland support files
if [ -d ${EHS_PLATFORM_APPLAND} ]; then
    echo "Copy appland installer support files ..."
    test -f ${EHS_PLATFORM_APPLAND}/INSTALLER.html && cp ${EHS_PLATFORM_APPLAND}/INSTALLER.html ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.html
    test -f ${EHS_PLATFORM_APPLAND}/info && cp ${EHS_PLATFORM_APPLAND}/info ${EHS_TARGET_APPLAND}/${INST_DEPLOY_FILE_NAME}.info
    # copy any other resources e.g. .jpg, .png etc.
    test -d ${EHS_PLATFORM_APPLAND}/res && cp -r ${EHS_PLATFORM_APPLAND}/res/* ${EHS_TARGET_APPLAND}/
fi

echo "Uploading to the appland server ..."
# make sure the directory structure for the run-time exists
ssh -p 8822  ${INST_SERVER_UNAME}@${INST_SERVER_URL} "mkdir -p \"$INST_SERVER_PATH\""
# copy installer and all resource files
scp -P 8822 ${EHS_TARGET_APPLAND}/* ${INST_SERVER_UNAME}@${INST_SERVER_URL}:${INST_SERVER_PATH}/

echo "Done!"

echo "#########################################################################"
