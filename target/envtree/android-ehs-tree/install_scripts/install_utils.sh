#!/bin/bash

source $ANDROID_VERSION_DIR/install_utils.sh

install_platform(){
    # set-up platform
    echo "Set-up everything in $SET_UP_PLATFORM_DIR"
    pushd $SET_UP_PLATFORM_DIR || exit 1
    chmod +x ./setup.sh
    ./setup.sh
    popd
}

install_product(){
    # install product related things
    echo "Installing product ($INSTALL_PRODUCT_DIR)"
    pushd $INSTALL_PRODUCT_DIR || exit 1
    chmod +x ./install.sh || exit 1
    ./install.sh || exit 1
    popd
}

install_android(){
    # install android related things
    echo "Installing scripts for a specific android version ($ANDROID_VERSION_DIR)"
    pushd $ANDROID_VERSION_DIR || exit 1
    chmod +x ./install.sh || exit 1
    ./install.sh || exit 1
    popd
}    

install_server(){
    # install server related things
    echo "Installing server configuration ($INSTALL_SERVER_DIR)"
    pushd $INSTALL_SERVER_DIR || exit 1
    if [ -f "./install.sh" ]; then
    	chmod +x ./install.sh || exit 1
    	./install.sh || exit 1
    else
    	echo "Server configuration - not available."	
    fi
    popd
}

# pack all of the supervisor scripts
if [ -n "$PACK_DIR" ]; then
    export ADB=
    # override
    shell_push(){
        SCR=$1
        DST=${PACK_DIR}$2
        if [ -d "$SCR" ]; then
            echo "Copying dir : $SCR =====> $DST"
            parentdir="$(dirname "$DST")"
            mkdir -p $parentdir || exit 1
            cp -r ${SCR} ${DST} || exit 1
        elif [ -f "$SCR" ]; then
            echo "Copying file : $SCR =====> $DST"
            parentdir="$(dirname "$DST")"
            mkdir -p $parentdir || exit 1
            cp ${SCR} ${DST} || exit 1
        else
            echo "FAILED to copy ($SCR). Source must be either a file or a directory."
        fi    
    }
    # override
    write_server_config(){
    	NAME=$1
    	URL=$2
    	echo "export DEVMAN_SERVER_NAME=\"${NAME}\"" >> ${PACK_DIR}/system/etc/ehs_supervisor/ehs_server_configure.sh || exit 1
    	echo "export SERVER_ADDRESS=\"${URL}\"" >> ${PACK_DIR}/system/etc/ehs_supervisor/ehs_server_configure.sh || exit 1
    }
    # override
    install_certs(){ 
        echo "ignore - install certs"
    }
    # override
    shell_exec(){
        echo "ignore - shell exec"
    }
    # override
    shell_push_ex(){
        echo "ignore - shell push extended"
    }
    # override
    uninstall(){
        echo "ignore - uninstall"
    }

    package_supervisor(){
        echo "=========== Package Supervisor Scripts ============="
        ROOT_DIR=${INSTALL_DIR}/root-dir
        pushd $ROOT_DIR || exit 1
        shell_push system / || exit 1
        popd
        # as 'shell_push' is overriden this function will be used to copy all files
        install_platform || exit 1
        install_product  || exit 1
        install_android  || exit 1
        install_server   || exit 1
    }
fi
