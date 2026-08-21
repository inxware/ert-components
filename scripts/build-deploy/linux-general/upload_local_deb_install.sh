#!/bin/bash

#This is a temporary work-around for installing the eRT directory to a /home directory and adding it to the desktop starter.

SCRIPT_NAME=$(basename "$0")
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Destination parsing is shared with deployViaSsh.sh so both scripts take the
# same 'user@host' / '<user> <host>' syntax.
source "${SCRIPT_DIR}/ssh_target_common.sh"

usage() {
    cat <<EOF
${SCRIPT_NAME} - rsync the built .deb to a remote Linux device and apt install it.

USAGE
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} <user@host> [target name] [TARGET_TREES dir]
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} <user> <host> [target name] [TARGET_TREES dir]

ARGUMENTS
    The device can be given either way - the same syntax deployViaSsh.sh
    accepts:

    <user@host>       user embedded in the address, e.g. pi@192.168.1.50
    <user> <host>     user and host as two separate arguments, user first

    Unlike deployViaSsh.sh there is no default user, because a bare first
    argument here cannot be told apart from the historic '<user> <host>' form.

    [target name]       optional, e.g. linux_arm64_lvgl_gg_debian11.  Defaults
                        to TARGET from the build system, else a built-in value.
    [TARGET_TREES dir]  optional, defaults to ../TARGET_TREES

OPTIONS
    -h, -help, --help   Show this help and exit.

EXAMPLES
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} pi@192.168.1.50
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} pi 192.168.1.50
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} pi@192.168.1.50 linux_arm64_lvgl_gg_debian11

EXIT STATUS
    0  success (or --help)
    2  bad usage - missing/malformed target
EOF
}

usage_error() {
    echo "ERROR: $1" >&2
    echo "" >&2
    usage >&2
    exit 2
}

case "$1" in
    -h|-help|--help)
        usage
        exit 0
        ;;
esac

# No EHS_SSH_DEFAULT_USER here on purpose - see the note in the help above.
if ! ehs_resolve_ssh_target "$1" "$2"; then
    usage_error "${EHS_SSH_ERROR}"
fi
shift ${EHS_SSH_SHIFT}

EHS_DEBIAN_LINUX_USER="${EHS_SSH_USER}"
EHS_DEBIAN_LINUX_IP="${EHS_SSH_HOST}"
EHS_DEBIAN_LINUX_TARGET=$1
TARGET_TREES_DIR=$2

echo "Uploading to ${EHS_SSH_DEST}"

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


