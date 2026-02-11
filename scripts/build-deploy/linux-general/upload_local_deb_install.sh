#!/bin/bash

#This is a temporary work-around for installing the eRT directory to a /home directory and adding it to the desktop starter.

if [ "$1" = "-h" ]; then
    echo "upload_local_deb_install.sh [ssh user] [ssh ip] optional:[target name] [TARGET_TREES dir]"
    exit
fi

EHS_DEBIAN_LINUX_USER=$1
EHS_DEBIAN_LINUX_IP=$2
EHS_DEBIAN_LINUX_TARGET=$3
TARGET_TREES_DIR=$4

if [ -z "$EHS_DEBIAN_LINUX_USER" ]; then
    echo "SSH user must be specify as a first argument. Use '-h' for more info."
    exit 1
fi

if [ -z "$EHS_DEBIAN_LINUX_IP" ]; then
    echo "SSH host must be specify as a second argument. Use '-h' for more info."
    exit 1
fi

if [ -z "$EHS_DEBIAN_LINUX_TARGET" ]; then
    if [ "${TARGET}" = "" ]; then	
    	TARGET=ehs_deb-linux_arm64_lvgl_gg_debian11
    else 
        echo "Using TARGET from build system: $TARGET"
    fi
else
    TARGET=ehs_deb-${EHS_DEBIAN_LINUX_TARGET}
fi

if [ -z "$TARGET_TREES_DIR" ]; then
    EHS_DEBIAN_DEB_SRC=../TARGET_TREES/${TARGET}
else
    EHS_DEBIAN_DEB_SRC=${TARGET_TREES_DIR}/${TARGET}
fi

# clear directory
if [ 1 = 0 ];then
# This wont work now the TARGET TREES is pointing to debs not staging directory. 
# old method of making it appear in the desktop instantly if we are ot using a deb installer.
echo "[Desktop Entry]
Name=eRT
Comment=Start eRT application
Exec=/home/${EHS_DEBIAN_LINUX_USER}/ert/${TARGET}/bin/run_ehs.sh
Terminal=false
Type=Application" > ehs.desktop

   ssh ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP} -t 'pkill ehs.exe ; mkdir -p /home/'${EHS_DEBIAN_LINUX_USER}'/ert ; sudo chmod a+w /etc/xdg/autostart ; exit'
   rsync -r ${EHS_DEBIAN_DEB_SRC}/*deb ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP}:/home/${EHS_DEBIAN_LINUX_USER}/ert && rsync ehs.desktop ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP}:/etc/xdg/autostart/ehs.desktop && \
   ssh ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP} -t 'cd /home/'${EHS_DEBIAN_LINUX_USER}'/ert/'${TARGET}'/bin; DISPLAY=:0 ./ehs.exe ; exit'
rm -f ehs.desktop
else
#INstall the apackage
   rsync -r ${EHS_DEBIAN_DEB_SRC}/*.deb ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP}: && \
   ssh -t ${EHS_DEBIAN_LINUX_USER}@${EHS_DEBIAN_LINUX_IP}  'cd ${HOME} && sudo apt install ./*.deb ; rm *.deb && killall ehs.exe ; cd /opt/ehs/bin && nohup ./ehs.exe &'
fi


