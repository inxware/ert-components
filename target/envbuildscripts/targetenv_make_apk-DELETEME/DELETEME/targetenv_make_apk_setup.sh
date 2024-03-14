#!/bin/bash

SetupTargetEnv(){
    
    EHS_ANDROID_LIB_FILE="$EHS_ROOT/ehs_$SPECIFIC_TARGET.so"
    
    if ! [ -f "$EHS_ANDROID_LIB_FILE" ] ; then
        echo "EHS lib file ($EHS_ANDROID_LIB_FILE) is not present in the root of EHS repository."
        echo "Make sure you build the EHS project for this traget ($SPECIFIC_TARGET) first."
        exit 1
    fi
    if ! [ -d "$ANDROID_STUDIO_ROOT" ]; then
        echo "TARGET_TREES must contain android_studio_project directory."
        exit 1
    fi    
    if ! [ -d "$REPOSITORY_ANDROID_STUDIO_ROOT" ] ; then
        echo "Android Studio project path must be specified in the hack file."
        echo "Please set REPOSITORY_ANDROID_STUDIO_ROOT "
        exit 1
    fi
    echo "Copying Android Studio project ($REPOSITORY_ANDROID_STUDIO_ROOT) to $ANDROID_STUDIO_ROOT"
    cp -r $REPOSITORY_ANDROID_STUDIO_ROOT/* $ANDROID_STUDIO_ROOT || exit 1
    if [ -z "$ANDROID_STUDIO_JNILIBS_PATH" ] ; then
        echo "Android Studio project JniLibs path must be specified."
        exit 1
    fi
    if ! [ -d "$ANDROID_STUDIO_JNILIBS_PATH" ] ; then
        mkdir -p $ANDROID_STUDIO_JNILIBS_PATH || exit 1
    fi
    echo "Copy .so lib to Android Studio project JniLibs ($ANDROID_STUDIO_JNILIBS_PATH)"
    cp "$EHS_ANDROID_LIB_FILE" "$ANDROID_STUDIO_JNILIBS_PATH/libnative-activity.so" || exit 1
    mkdir -p ${ANDROID_STUDIO_DEVMAN_PATH} || exit 1
    mkdir -p ${ANDROID_STUDIO_DEVMAN_PATH}/config || exit 1
    mkdir -p ${ANDROID_STUDIO_DEVMAN_PATH}/certs || exit 1
}

#Obsolete: 
#SetupTargetEnv_Url(){    
#    ANDROID_STUDIO_DEVMAN_CONFIG_PATH=${ANDROID_STUDIO_DEVMAN_PATH}/config
#    if ! [ -d $ANDROID_STUDIO_DEVMAN_CONFIG_PATH ]; then
#        echo "EHS devman url directory is not present ($ANDROID_STUDIO_DEVMAN_CONFIG_PATH)"
#        exit 1
#    fi
#    # add url file
#    if [ -n "$DEVMAN_SERVER_URL" ]; then
#        echo "${DEVMAN_SERVER_URL}" | tr -d \\n > "$ANDROID_STUDIO_DEVMAN_CONFIG_PATH/DEVMANURL.000"
#        echo "Configuring EHS url ($( cat $ANDROID_STUDIO_DEVMAN_CONFIG_PATH/DEVMANURL.000 ))"
#    fi
#}