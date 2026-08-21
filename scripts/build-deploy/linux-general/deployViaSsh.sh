#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
# not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

SCRIPT_NAME=$(basename "$0")
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Destination parsing is shared with upload_local_deb_install.sh so both scripts
# take the same 'user@host' / '<user> <host>' syntax.
source "${SCRIPT_DIR}/ssh_target_common.sh"

usage() {
    cat <<EOF
${SCRIPT_NAME} - deploy the built ehs.exe to a remote Linux device over SSH/SCP.

USAGE
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} [option] <user@host>
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} [option] <user> <host>
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} [option] <host>

    Run from the ert-components repo ROOT - it reads TARGET.cfg from the current
    directory and copies out of ../TARGET_TREES/ehs_env-\${TARGET}/.

OPTIONS
    -console      Stop the running app, copy ehs.exe, then start it via
                  run_ehs.sh with NO_RESTART LIB_HOST so the console output
                  stays attached to this terminal.
    -noexe        Copy ehs.exe only.  Does not stop the running app and does
                  not start anything afterwards.
    -h, -help, --help
                  Show this help and exit.

    With no option, the running app is stopped, ehs.exe is copied, and it is
    restarted with DISPLAY=:0 and LD_LIBRARY_PATH=/opt/ehs/bin/cslib.

ARGUMENTS
    The device can be given in any of three ways - the same syntax that
    upload_local_deb_install.sh accepts:

    <user@host>       user embedded in the address, e.g. pi@192.168.1.50
    <user> <host>     user and host as two separate arguments, user first
    <host>            no user given - defaults to root

    Whichever account is used needs to be able to write to /opt/ehs/bin and
    (unless -noexe) to run 'service run_ehs stop' and killall - so a non-root
    user needs passwordless sudo, or the deploy will fail.

EXAMPLES
    # no user given - connects as root
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} 192.168.1.50

    # user in the address
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} pi@192.168.1.50
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} -console pi@raspberrypi.local
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} -noexe admin@mydevice.local

    # user as a separate argument - identical to the two above
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} pi 192.168.1.50
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} -console pi raspberrypi.local

    # explicit root is fine too, and identical to giving no user
    ./scripts/build-deploy/linux-general/${SCRIPT_NAME} -console root@192.168.1.50

EXIT STATUS
    0  success (or --help)
    2  bad usage - unknown option, missing/extra argument, malformed address
EOF
}

# Print the error, then the help, and bail out.  Errors go to stderr so they
# stay visible if the help itself is being piped somewhere.
usage_error() {
    echo "ERROR: $1" >&2
    echo "" >&2
    usage >&2
    exit 2
}

DO_EXE=1
WITHCONSOLE=0

case "$1" in
	-h|-help|--help)
		usage
		exit 0
		;;
	-console)
		WITHCONSOLE=1
		shift
		;;
	-noexe)
		DO_EXE=0
		shift
		;;
	-*)
		usage_error "unknown option '$1'"
		;;
esac

# Historically root was hardcoded into every ssh/scp line below.  It is now
# only the fallback for an address that names no user.
EHS_SSH_DEFAULT_USER="root"
if ! ehs_resolve_ssh_target "$1" "$2"; then
	usage_error "${EHS_SSH_ERROR}"
fi
shift ${EHS_SSH_SHIFT}

if [ "$#" -gt 0 ]; then
	usage_error "unexpected extra argument '$1'"
fi

SSH_DEST="${EHS_SSH_DEST}"
echo "Deploying to ${SSH_DEST}"

source TARGET.cfg

#make targetenv_deb UPLOAD="ambifier@www.ambifier.com:/home/ambifier/deb-repo"
if [ "$DO_EXE" == "1" ]; then

   ssh ${SSH_DEST} service run_ehs stop
   sleep 2
   ssh ${SSH_DEST} killall run_ehs.sh ; killall ehs.exe
fi

if [ 1 = 0 ]; then
echo "WARNING!!!!! Hack to send the appp is active"
ssh  ${SSH_DEST} "mkdir -p /opt/ehs/appdata/default/" || exit
scp ../TARGET_TREES/ehs_env-${TARGET}/appdata/default/* ${SSH_DEST}:/opt/ehs/appdata/default/ || exit
fi

echo "copying ehs.exe ..."
scp ../TARGET_TREES/ehs_env-${TARGET}/bin/ehs.exe ${SSH_DEST}:/opt/ehs/bin/ || exit
echo "Done"

#Hack this to install the debian package
if [ 1 = 0 ]; then
if [ -f  ../TARGET_TREES/ehs_deb-${TARGET}/*.deb ]; then
# thenls ../TARGET_TREES/ehs_deb-${TARGET}/*.deb  ]; then
echo "copying debian_package ..."
scp ../TARGET_TREES/ehs_deb-${TARGET}/*.deb ${SSH_DEST}:/opt/ehs/ || exit
echo "Done"
fi
fi

if [ "$DO_EXE" = "1" ]; then
  if [ "$WITHCONSOLE" = "1" ]; then
    ssh ${SSH_DEST} "cd /opt/ehs/bin && ./run_ehs.sh NO_RESTART LIB_HOST"
  else
#    ssh ${SSH_DEST} "service run_ehs start"
    ssh ${SSH_DEST} "cd /opt/ehs/bin && export DISPLAY=:0; export LD_LIBRARY_PATH=/opt/ehs/bin/cslib && ./ehs.exe"
  fi
fi
