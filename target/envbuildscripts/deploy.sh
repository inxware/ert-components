#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2008-2026, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# deploy.sh <user>@<ipaddress>
#
# Deploys the staged TARGET_TREES output for the current platform to a remote
# device over SSH:
#   1. Kills any running run_ehs.sh and ehs.exe processes on the device
#   2. Rsyncs or scp ../TARGET_TREES/ehs_env-<TARGET>/ to /opt/ehs/ on the device
#   3. Starts ehs via /opt/ehs/bin/run_ehs.sh (defaults: NO_RESTART LIB_HOST)

set -e

source ./scripts/build-function-library/colour.sh
source ./TARGET.cfg

DEPLOY_TARGET="$1"

if [ "$SSH_PORT" != "" ]; then
   EXTRA_SCP_ARGS="-P ${SSH_PORT}"
   EXTRA_SSH_ARGS="-p ${SSH_PORT}"
fi



if [ -z "$DEPLOY_TARGET" ]; then
    err "Usage: ./configure -deploy <username>@<ipaddress>"
    exit 1
fi

# Basic sanity check on the argument format
if [[ ! "$DEPLOY_TARGET" =~ ^[^@]+@[^@]+$ ]]; then
    err "Invalid target '${DEPLOY_TARGET}' — expected format: <username>@<ipaddress>"
    exit 1
fi

STAGING_DIR="../TARGET_TREES/ehs_env-${TARGET}"

if [ ! -d "$STAGING_DIR" ]; then
    err "Staging directory ${STAGING_DIR} does not exist."
    echo "Run ${TXT_FG_WHITE}make targetenv${TXT_FG} first to assemble the runtime tree."
    exit 1
fi

echo "--------------------------------------------------------------------"
echo
echo "${TXT_FG_CYAN}Deploying ${TXT_FG_WHITE}${TARGET}${TXT_FG_CYAN} to ${TXT_FG_WHITE}${DEPLOY_TARGET}${TXT_RESET}"
echo

echo "${TXT_FG_WHITE}[1/3] Stopping running ehs processes on ${DEPLOY_TARGET}...${TXT_RESET}"
ssh ${EXTRA_SSH_ARGS} "$DEPLOY_TARGET" "pkill -f run_ehs.sh; pkill -f ehs.exe; sleep 1; true" 2>/dev/null || true

echo "${TXT_FG_WHITE}[2/3] Syncing ${STAGING_DIR}/ -> ${DEPLOY_TARGET}:/opt/ehs/ ...${TXT_RESET}"
if [ 1 = 0 ]; then
#if command -v rsync > /dev/null 2>&1; then
	rsync -av -e ssh ${EXTRA_SSH_ARGS} "${STAGING_DIR}/" "${DEPLOY_TARGET}:/opt/ehs/"
else
	scp ${EXTRA_SCP_ARGS} -r "${STAGING_DIR}/." "${DEPLOY_TARGET}:/opt/ehs/"
fi
echo "${TXT_FG_WHITE}[3/3] Starting ehs on ${DEPLOY_TARGET} (Ctrl+C to detach)...${TXT_RESET}"
echo "--------------------------------------------------------------------"
ssh "$DEPLOY_TARGET" "/opt/ehs/bin/run_ehs.sh"
