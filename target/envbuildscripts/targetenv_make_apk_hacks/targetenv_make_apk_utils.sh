#!/bin/bash

CancelFailed(){
    echo "FAILED !!!"
    rm -rf ${TARGET_PATH}
	exit 1
}

GetApplicationRepo(){
    TOOLS_DIR=$1
    echo "Get scheduler application repository (apps.git) ===> ($TOOLS_DIR)"
    if [ -d "$TOOLS_DIR" ];then
        pushd ${TOOLS_DIR} || CancelFailed
        git checkout RELEASE-PRODUCTION || CancelFailed
        git pull  || CancelFailed
        popd
    else
        pushd .. || CancelFailed
        git clone ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/apps.git || CancelFailed
        popd
        pushd ${TOOLS_DIR} || CancelFailed
        git checkout  RELEASE-PRODUCTION || CancelFailed
        popd
    fi
    if ! [ -d "$TOOLS_DIR" ]; then
        echo "Failed to obtain the tools repository (apps.git). Make sure the repository exits ($TOOLS_DIR)"
        CancelFailed
    fi
}

SshKeyFile(){
    if [ -n "$DEVMAN_SERVER_DOMAIN" ] && [ -n "$DEVMAN_UNAME" ]; then
        KEY_NAME=$(echo "$DEVMAN_SERVER_DOMAIN" | sed -r 's/[.]+/_/g')
        KEY_NAME=${DEVMAN_UNAME}"_"${KEY_NAME}
        SSH_FILE="$HOME/.ssh/$KEY_NAME"
        echo $SSH_FILE
    fi
}

SetupSshKey(){
    # generate ssh key
    SSH_FILE=$1
    if [ -n "$SSH_FILE" ]; then
        echo "Settup SSH key for Devman Server URL"
        if test -f "$SSH_FILE"; then
            echo "key $SSH_FILE exists"
        else
            echo "ssh Key ($SSH_FILE) doesn't exist"
            echo "creating SSH key pair for the devman server"
            ssh-keygen -N "" -f $SSH_FILE
        fi
        echo "Add key to ($DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN)"
        ssh-copy-id -i ${SSH_FILE} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    fi

}

CreateDevmanSupervisorUpdatesData(){
    echo "Creating supervisor patch data for ($SPECIFIC_TARGET)"
    EHS_UPDATES_SCRIPT=${EHS_ROOT}/target/envtree/android-ehs-tree/devman/updates/platform/${SYSTEM_VARIANT}/update-supervisor.sh
    if ! [ -f "$EHS_UPDATES_SCRIPT" ]; then
        echo "Failed to upload syspatch, the updates script doesn't exists ($EHS_UPDATES_SCRIPT)"
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
    echo  "Creating syspatch data for ($SPECIFIC_TARGET)"
    EHS_UPDATES_SCRIPT=${EHS_ROOT}/target/envtree/android-ehs-tree/devman/updates/product/${EHS_PRODUCT_NAME}/dldata.sh
    if ! [ -f "$EHS_UPDATES_SCRIPT" ]; then
        echo "Failed to upload syspatch, the updates script doesn't exists ($EHS_UPDATES_SCRIPT)"
        exit 1
    fi
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/apps
    EHS_TARGET_BIN=${TARGET_PATH}/bin
    mkdir -p ${TARGET_SYSPATCH_APPS} || exit 1
    pushd ${TARGET_SYSPATCH_APPS} || exit 1
    tar -zcvf dldata.tgz -C ${EHS_TARGET_BIN} . || exit 1
    popd
    cp ${EHS_UPDATES_SCRIPT} ${TARGET_SYSPATCH_APPS} || exit 1
}

UploadDevmanAppUpdatesData(){
    SSHKEY=$1
    echo "Uploading apps syspatch data for ($SPECIFIC_TARGET)"
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/apps
    EHS_UPDATES_SCRIPT=${TARGET_SYSPATCH_APPS}/dldata.sh
    EHS_UPDATES_DATA=${TARGET_SYSPATCH_APPS}/dldata.tgz
    SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"
    if [ "$BUILD_MODE" = "ci" ]; then
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}-ci/"
    elif [ "$BUILD_MODE" = "debug" ]; then
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}-debug/"
    else
        EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}/"
    fi
    echo "SSHKEY ======> $SSHKEY"
    echo "ADDRESS =====> $DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    echo "LOCATION ====> $EHS_UPDATES_SERVER_DIR"
    ssh ${SSHKEY} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN" 'mkdir -p "'${EHS_UPDATES_SERVER_DIR}'"' || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_DATA} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_SCRIPT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
}

UploadDevmanSupervisorUpdatesData(){
    SSHKEY=$1
    echo "Uploading supervisor syspatch data for ($SPECIFIC_TARGET)"
    TARGET_SYSPATCH_APPS=${TARGET_SYSPATCH}/supervisor
    EHS_UPDATES_SCRIPT=${TARGET_SYSPATCH_APPS}/dldata.sh
    EHS_UPDATES_DATA=${TARGET_SYSPATCH_APPS}/dldata.tgz
    SERVER_SYSPATCH_DIR="/home/inx-devman/bin/dldata-scripts/system-patches"
    EHS_UPDATES_SERVER_DIR="$SERVER_SYSPATCH_DIR/update-android-supervisor-${SYSTEM_VARIANT}-${EHS_PRODUCT_NAME}/"

    echo "SSHKEY ======> $SSHKEY"
    echo "ADDRESS =====> $DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN"
    echo "LOCATION ====> $EHS_UPDATES_SERVER_DIR"
    ssh ${SSHKEY} -p ${SSHPORT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN" 'mkdir -p "'${EHS_UPDATES_SERVER_DIR}'"' || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_DATA} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
    scp ${SSHKEY} -P ${SSHPORT} ${EHS_UPDATES_SCRIPT} "$DEVMAN_UNAME@$DEVMAN_SERVER_DOMAIN:$EHS_UPDATES_SERVER_DIR" || exit 1
}
