#!/bin/bash
# inx limited 2020
# Creates an android supervisor bundle. Assembles some scripts that run on the target 
# and puts them somehere - hopefully in the staging directory (TBC)

set -e

export SPECIFIC_TARGET=$1
export NO_RUN=$2
export EHS_ROOT=`pwd` # assuming we're in the ehs project root
pushd ${EHS_ROOT}/..
export REPOSITORY_ROOT=`pwd`
popd

if [ "${NO_RUN}" = "" ]; then
echo "**************************************************************************************"
echo "**  Making Android Deployment Package for ($SPECIFIC_TARGET) **"
echo "**************************************************************************************"
fi

pushd ../TARGET_TREES || exit 1
TARGET_TREES=$(pwd)
popd
if ! [ -d "$TARGET_TREES" ]; then
    echo "TARGET_TREES directory does not exists. Make sure 'make targetenv' is run"
    exit 1
fi
export TARGET_PATH=${TARGET_TREES}/ehs_env-${SPECIFIC_TARGET}

if [ "${BUILD_WITH_ANDROID_SUPERVISOR}" = "" ]; then
    echo "WARNING: Not building and deploying the Android OS supervisor." 
else
    export BUILD_WITH_ANDROID_SUPERVISOR
fi

if [ "${DEVMAN_SERVER_NAME}" = "" ]; then
    echo "DEVMAN_SERVER_NAME is not set !!!"
    exit 1
else 
    export DEVMAN_SERVER_NAME
    echo "======================================================================================"
    echo "[DEVMAN_SERVER_NAME=$DEVMAN_SERVER_NAME]"
    echo "======================================================================================"
fi

if [ "${DEVMAN_SERVER_DOMAIN}" = "" ]; then
    echo "DEVMAN_SERVER_DOMAIN is not set!!!"
    exit 1
else
    export DEVMAN_SERVER_DOMAIN
fi

if [ "${DEVMAN_SERVER_PROTOCOL}" = "" ]; then
    echo "DEVMAN_SERVER_PROTOCOL is not set!!!"
    exit 1
else
    export DEVMAN_SERVER_PROTOCOL

fi

if [ "${SSHPORT}" = "" ]; then
    export SSHPORT=22
    echo "SSHPORT is not set. Setting it to default ($SSHPORT)."
fi
echo "======================================================================================"
echo "[SSHPORT=$SSHPORT]"
echo "======================================================================================"

export DEVMAN_SERVER_URL="${DEVMAN_SERVER_PROTOCOL}://${DEVMAN_SERVER_DOMAIN}"
echo "======================================================================================"
echo "[DEVMAN_SERVER_URL=$DEVMAN_SERVER_URL]"
echo "======================================================================================"

#todo2023 - the following shouldbe the same as for debian and other server uploads.
if [ "${DEVMAN_UNAME}" = "" ]; then
	#export DEVMAN_UNAME="inx"
    echo "WARNING: DEVMAN_UNAME is not set"
else
    export DEVMAN_UNAME
    echo "======================================================================================"
    echo "[DEVMAN_UNAME=$DEVMAN_UNAME]"
    echo "======================================================================================"
fi

if [ "${EHS_PRODUCT_NAME}" = "" ]; then
    echo "WARNING: EHS_PRODUCT_NAME is not set"
else
    export EHS_PRODUCT_NAME
    echo "======================================================================================"
    echo "[EHS_PRODUCT_NAME=$EHS_PRODUCT_NAME]"
    echo "======================================================================================"
fi

if [ "${SYSTEM_VARIANT}" = "" ]; then
    echo "WARNING: SYSTEM_VARIANT is not set"
else
    export SYSTEM_VARIANT
    echo "======================================================================================"
    echo "[SYSTEM_VARIANT=$SYSTEM_VARIANT]"
    echo "======================================================================================"
fi

if [ "${EHS_ANDROID_INSTALL_VERSION}" = "" ]; then
    echo "WARNING: EHS_ANDROID_INSTALL_VERSION is not set"
else
    export EHS_ANDROID_INSTALL_VERSION
    echo "======================================================================================"
    echo "[EHS_ANDROID_INSTALL_VERSION=$EHS_ANDROID_INSTALL_VERSION]"
    echo "======================================================================================"
fi

# Yes these paths are in the staging directory and not pulled out are ass again
export SUPERVISOR_PACKAGE_PATH=${TARGET_PATH}/supervisor
export TARGET_SYSPATCH=${TARGET_PATH}/syspatch

CreateDevmanSupervisorUpdatesData(){
    echo "Creating supervisor patch data for ($SPECIFIC_TARGET)"
    EHS_UPDATES_SCRIPT=${EHS_ROOT}/target/envbuildscripts/installers/android-adb/devman/updates/platform/${SYSTEM_VARIANT}/update-supervisor.sh
    if ! [ -f "$EHS_UPDATES_SCRIPT" ]; then
        echo "Failed to upload supervisor syspatch, the updates script doesn't exists ($EHS_UPDATES_SCRIPT)"
        exit 1
    fi
    EHS_TARGET_SUPERVISOR=${TARGET_PATH}/supervisor
    SUPERVISOR_PATCH_DATA=${TARGET_SYSPATCH}/supervisor
    mkdir -p ${SUPERVISOR_PATCH_DATA} || exit 1
    EHS_DOWNLOADER=${EHS_ROOT}/target/envtree/android-ehs-tree/utils/downloader.apk
    cp ${EHS_DOWNLOADER} ${EHS_TARGET_SUPERVISOR} || exit 1
    pushd ${SUPERVISOR_PATCH_DATA} || exit 1
    tar -zcvf dldata.tgz -C ${EHS_TARGET_SUPERVISOR} . || exit 1
    popd
    cp ${EHS_UPDATES_SCRIPT} ${SUPERVISOR_PATCH_DATA}/dldata.sh || exit 1
}

CreateDevmanAppUpdatesData(){
    if [ "${EHS_PRODUCT_NAME}" != "" ]; then
        echo  "Creating Devman syspatch package for ($SPECIFIC_TARGET) as product ${EHS_PRODUCT_NAME}"
        EHS_UPDATES_SCRIPT=${EHS_ROOT}/target/envbuildscripts/installers/android-adb/devman/updates/product/${EHS_PRODUCT_NAME}/dldata.sh
        if ! [ -f "$EHS_UPDATES_SCRIPT" ]; then
            echo "Failed to upload App syspatch, the updates script doesn't exists ($EHS_UPDATES_SCRIPT)"
            exit 1
        fi
        TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/apps
        EHS_TARGET_BIN=${TARGET_PATH}/bin
        mkdir -p ${TARGET_SYSPATCH_APPS} || exit 1
        pushd ${TARGET_SYSPATCH_APPS} || exit 1
        tar -zcvf dldata.tgz -C ${EHS_TARGET_BIN} . || exit 1
        popd
        cp ${EHS_UPDATES_SCRIPT} ${TARGET_SYSPATCH_APPS} || exit 1
    else
        echo "WARNING: Could not create Devman syspatch (dldata.tgz) for ${SPECIFIC_TARGET}, no product name set"   
    fi
}

# Example Arguments:
# (1) ssh://git@github.com:inxware/ert-components.git
# (2) ../my_applications/nice-application.apk
SetupTargetEnv_AddSupplementaryApps(){
    if [ "$1" = "" -o "$2" = "" ]; then
        echo "Error you need to provide the App Repo and Path as two arguments for SetupTargetEnv_AddSupplimentaryApps"
        exit 1
    fi
    AppRepoUrl=$1
    AppRepoPath=$2
    AppRepoRoot=$(echo "$AppRepoPath" | cut -d "/" -f1)
    echo "Setting up bin folder for $AppRepoPath "
    APP_REPO_DIR=${EHS_ROOT}/../${AppRepoRoot}
    echo "Is repository is already present at ($APP_REPO_DIR) ?"
    echo "Enter password for '${AppRepoUrl}' repository when prompted."
    if [ -d "$APP_REPO_DIR" ]; then
        pushd ${APP_REPO_DIR} || exit 1
        git pull || exit 1
        popd
    else
        pushd ${EHS_ROOT}/.. || exit 1
        git clone ${AppRepoUrl} || exit 1
        popd
    fi
    APP_APK=${EHS_ROOT}/../${AppRepoPath}
    if ! [ -f "$APP_APK" ]; then
        echo "Failed to obtain ${APP_APK} from the repo ${AppRepoUrl}/${AppRepoPath}"
        exit 1
    fi
    cp ${APP_APK} ${TARGET_PATH}/bin/ || exit 1
    echo "Supplementary app OK !"
}


if [ "${NO_RUN}" = "" ]; then
    echo "======================================================================================"
    echo "Obtain all supplementary apps."
    if [ "$ANDROID_SUPPLEMENTARY_APP_REPO" = "" ]; then
        echo "ANDROID_SUPPLEMENTARY_APP_REPO is not specified for this target."
    else
        echo "Getting supplemntatry app from ( ${ANDROID_SUPPLEMENTARY_APP_REPO} )"
        SetupTargetEnv_AddSupplementaryApps "${ANDROID_SUPPLEMENTARY_APP_REPO}" "${ANDROID_SUPPLEMENTARY_APP_PATH}"
    fi
    echo "======================================================================================"
    echo "Create supervisor directory."
    test -d "${SUPERVISOR_PACKAGE_PATH}" && rm -rf ${SUPERVISOR_PACKAGE_PATH}
    mkdir ${SUPERVISOR_PACKAGE_PATH}
    # use this script to package up supervisor
    ${EHS_ROOT}/target/envbuildscripts/installers/android-adb/adb_targetenv.sh ${SPECIFIC_TARGET} ${EHS_ROOT}
    echo "======================================================================================"
    echo "Create devman updates supervisor data"
    CreateDevmanSupervisorUpdatesData
    echo "======================================================================================"
    echo "Create devman updates app data"
    CreateDevmanAppUpdatesData
    echo "======================================================================================"
fi