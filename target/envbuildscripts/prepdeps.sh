#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2010-2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
# <https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# This script retrieves all support components built for the target type defined by the $OS and $ARCH
# This includes installing base host tools and the inxware dependency repos.
# This script should only be called via make prepdeps to setup the appropriate encironment variables.

set -e

source ./scripts/build-function-library/colour.sh
TXT_FG=${TXT_FG_GREEN}

if [ ! -n ${EHS_GNU_OS} ];then
    err "Environment variable EHS_GNU_OS is not set - exiting."
    exit 1
fi

if [ ! -n ${EHS_GNU_ARCH} ];then
    err "Environment variable ARCH is not set - exiting."
    exit 1
fi

# Is PWD inside a git repo, and is the remote a GitHub repo?
IS_PUBLIC="no"
REPO_URL=""
if command -v git >/dev/null 2>&1 && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    REPO_URL="$(git config --get remote.origin.url)"
    if [[ "${REPO_URL,,}" == *github.com* ]]; then
        IS_PUBLIC="yes"
        echo "${TXT_FG_BRIGHT_GREEN}Preparing dependencies from public source tree"
    else
        warn "Preparing dependencies from private source tree"
    fi
else
    err "'git' command not found or not in a git repository - exiting."
    exit 1
fi


# NOTE: host tool installation (Xcode CLT, Homebrew, apt packages, git-lfs, make,
# curl, Docker) used to live here. It is host scope, not target scope, and ran on
# every 'make prepdeps' - so a 37-platform regression sweep probed Homebrew and
# could invoke 'sudo apt' 37 times to do nothing. It now lives in
# target/envbuildscripts/configure_host.sh, run once per machine via
# 'make configure-host'.
#
# What stays here needs the make-exported environment that only 'make prepdeps'
# provides (TARGET, EHS_ARCH, EHS_GNU_OS...), which is why it is not in ./configure:
#   - the dependency-repo fetch (skippable with SKIP_REPOS=1)
#   - the per-target checks at the end of this file
#
OS_TYPE="$(uname -s)"
CPU_ARCH="$(uname -m)"


# Set up some hardwired paramters for the inxware dependency repos
# TODO2025: THIS SHOULD BE A BASE MAKE SYSTEM VARIABLE THAT IS EXPORTED TO HERE AND OTHER SCRIPTS
LOCAL_BASE="../"

if [ "$IS_PUBLIC" = "yes" ]; then
    REPOSITORY_BASE="$(dirname -- ${REPO_URL})"
else
#   REPOSITORY_BASE="git@github.com:inxware/"
    REPOSITORY_BASE="${REPO_URL}/../"
fi

#
# CORE PLATFORM SUPPORT FOR ERT-COMPONENTS & KERNEL BUILD
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
EHS_CORE_SUPPORT_REPO="ert-build-support.git"

# Removed 2026-08: EHS_CORE_SUPPORT_PATH_FROM_BASE / EHS_TOOLCHAIN_PATH_FROM_BASE /
# EHS_TOOLCHAIN_LOCAL_PATH were computed here and never read. One of them also
# contained "$(EHS_GNU_OS_ARCH)" - command substitution, a makefile $(VAR) written
# into a shell script - which ran a non-existent command and expanded to empty. The
# repo fetch below clones whole repos to fixed paths, so none of it was needed.
# If a per-arch path is ever wanted here, note that the fetch is arch-independent
# today and would need sparse-checkout / 'git lfs pull --include' to become arch-aware.
EHS_CORE_SUPPORT_SERVER_PATH="${REPOSITORY_BASE}/${EHS_CORE_SUPPORT_REPO}"
EHS_CORE_SUPPORT_LOCAL_PATH="${LOCAL_BASE}/${EHS_CORE_SUPPORT_DIR}"

EHS_KERNEL_REPO="EHS-kernel.git"
EHS_KERNEL_SERVER_PATH="${REPOSITORY_BASE}/${EHS_KERNEL_REPO}"
EHS_KERNEL_LOCAL_PATH="${LOCAL_BASE}/${EHS_KERNEL_REPO}"

#
# KERNEL HEADERS - FOUND IN THE ABOVE EHS-CORE SUPPORT REPO
#

#
# COMPONENT SUPPORT LIBRARIES FOR PLATFORM
#
EHS_COMPONENT_LIB_SUPPORT_DIR="ert-contrib-middleware"
EHS_COMPOENT_SUPPORT_REPO="${EHS_COMPONENT_LIB_SUPPORT_DIR}.git"
EHS_COMPONENT_SUPPORT_LOCAL_PATH="${LOCAL_BASE}/${EHS_COMPONENT_LIB_SUPPORT_DIR}/"
EHS_COMPONENT_SUPPORT_SERVER_PATH="${REPOSITORY_BASE}/${EHS_COMPOENT_SUPPORT_REPO}"

#
# GET THE STUFF!! if specifics have been given
#
if [ ! -n "${SKIP_REPOS}" ]; then
    echo "Retrieving latest ERT build support repo for ${EHS_GNU_OS_ARCH} from ${EHS_CORE_SUPPORT_SERVER_PATH}"
    if [ -e $EHS_CORE_SUPPORT_LOCAL_PATH ]; then
        echo "Found existing repo at '${EHS_CORE_SUPPORT_LOCAL_PATH}'. Updating..."
        pushd $EHS_CORE_SUPPORT_LOCAL_PATH
        git pull
        git lfs pull
        popd
    else
        echo "No local repo found at '${EHS_CORE_SUPPORT_LOCAL_PATH}'."
        echo "Cloning the ert-build-support repository $EHS_CORE_SUPPORT_SERVER_PATH to $EHS_CORE_SUPPORT_LOCAL_PATH"
        warn "This may take some time!"
        git clone --depth 1 $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
    fi

    echo "Retrieving latest ERT contrib middleware for ${EHS_GNU_OS_ARCH} from ${EHS_COMPONENT_SUPPORT_SERVER_PATH} ..."
    if [ -e $EHS_COMPONENT_SUPPORT_LOCAL_PATH ]; then
        echo "Found existing Repo at '${EHS_COMPONENT_SUPPORT_LOCAL_PATH}'. Updating..."
        pushd $EHS_COMPONENT_SUPPORT_LOCAL_PATH
        git pull
        git lfs pull
        popd
    else
        echo "No local repo found at '${EHS_COMPONENT_SUPPORT_LOCAL_PATH}'."
        echo "Cloning the ert-contrib-middleware repository $EHS_COMPONENT_SUPPORT_SERVER_PATH to $EHS_COMPONENT_SUPPORT_LOCAL_PATH"
        warn "This may take some time!"
        git clone --depth 1 $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
    fi

    if [ "$IS_PUBLIC" = "no" ]; then
        echo "Retrieving latest EHS kernel server for ${EHS_GNU_OS_ARCH} from ${EHS_COMPONENT_SUPPORT_SERVER_PATH} ..."
        if [ -e $EHS_KERNEL_LOCAL_PATH ]; then
            echo "Found existing repo at '${EHS_KERNEL_LOCAL_PATH}'. Updating..."
            pushd $EHS_KERNEL_LOCAL_PATH
            git pull
            git lfs pull
            popd
        else
            echo "No local repo found at '${EHS_KERNEL_LOCAL_PATH}'."
            echo "Cloning the ehs-kernel-server repository $EHS_KERNEL_SERVER_PATH to $EHS_KERNEL_LOCAL_PATH"
            warn "This may take some time!"
            git clone --depth 1 $EHS_KERNEL_SERVER_PATH $EHS_KERNEL_LOCAL_PATH
        fi
    fi

    echo
    echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
    echo
    echo "${TXT_FG_WHITE}Successfully retrieved dependencies for $EHS_GNU_OS_$EHS_GNU_ARCH"
    echo
    echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
    echo
else
    echo "Skipped dependency repo pull (SKIP_REPOS was set)"
fi

# macOS: the EHS kernel archive must be built natively on the host.
# On Linux the pre-built archive is distributed via ert-build-support (or built
# inside a Docker image by make all_docker). On macOS there is no Docker image
# for the kernel build, so it must be compiled from source in ../EHS-kernel.
if [ "$OS_TYPE" = "Darwin" ] && [ "$IS_PUBLIC" = "no" ]; then
    KERNEL_ARCHIVE="${LOCAL_BASE}/ert-build-support/support_libs/target_libs/x86_64-darwin/kernel/libehs_ehrt1.a"
    if [ ! -f "$KERNEL_ARCHIVE" ]; then
        echo
        echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
        warn "macOS: EHS kernel archive not found at:"
        echo "${TXT_FG_WHITE}  ${KERNEL_ARCHIVE}"
        echo
        echo "  Build it with:"
        echo "    cd ../EHS-kernel"
        echo "    ./configure macos_x86_64_clang_ehrt1"
        echo "    make all"
        echo "    cd ../ert-components"
        echo
        echo "  The archive is installed automatically by 'make all' in EHS-kernel."
        echo "  Then run 'make all' here to link the final binary."
        echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
        echo
    else
        echo "${TXT_FG_BRIGHT_GREEN}macOS: EHS kernel archive found at:"
        echo "${TXT_FG_WHITE}  ${KERNEL_ARCHIVE}"
    fi
fi

# XMOS XTC Tools — prompt the developer if the toolchain archive is missing.
# XTC Tools cannot be downloaded automatically (requires XMOS developer account
# and licence acceptance). The archive must be placed manually before running
# 'make build_docker_local'.
if [ "${EHS_ARCH}" = "xcore" ]; then
    XTC_ARCHIVE="${PWD}/target/platform/${TARGET}/xtc-tools-linux.tgz"
    if [ -f "${XTC_ARCHIVE}" ]; then
        echo "${TXT_FG_BRIGHT_GREEN}XMOS XTC Tools archive found at:"
        echo "${TXT_FG_WHITE}  ${XTC_ARCHIVE}"
        echo "${TXT_FG_BRIGHT_GREEN}Run 'make build_docker_local' to build the Docker image."
    else
        echo
        echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
        warn "XMOS target detected — XTC Tools archive not found."
        echo "${TXT_FG_WHITE}"
        echo "  XTC Tools must be downloaded manually from the XMOS website (requires a free developer account):"
        echo "    https://www.xmos.com/software-tools/"
        echo
        echo "  After downloading the Linux 64-bit archive:"
        echo "    1. Rename the file to:  xtc-tools-linux.tgz"
        echo "    2. Place it at:         ${XTC_ARCHIVE}"
        echo "    3. Run:                 make build_docker_local"
        echo "    4. Then build with:     make all_docker"
        echo
        echo "  Note: do NOT commit xtc-tools-linux.tgz to the repository (it is .gitignored)."
        echo "${TXT_FG_GREY}---------------------------------------------------------------------------------------------------------------------------"
        echo
    fi
fi

# Vagrant builds are no longer supported.
#
# Every platform that used to carry a Vagrantfile also has a Dockerimagename and
# builds through 'make all_docker'; the Vagrantfiles were dead from 2023 and were
# removed in 2026. What kept them alive was this block: it ran before the Docker
# branch and blocked on 'read -n 1' waiting for a keypress. Harmless interactively,
# fatal to CI - once SystemTests/CI started calling 'make prepdeps' per target
# (ci_functions.sh, April 2026) a single platform with a leftover Vagrantfile
# silently stalled the entire regression sweep, leaving a building.flag and no
# pass/fail verdict.
#
# So this is now a hard error rather than a prompt: a Vagrantfile in a platform
# directory is a mistake, and it must fail loudly and immediately instead of
# waiting for input that never comes in an automated run.
if  [ -f ${PWD}/target/platform/${1}/Vagrantfile ]; then
    echo "ERROR: target/platform/${1}/Vagrantfile found, but Vagrant builds are no longer supported." >&2
    echo "       Build this platform with Docker instead:" >&2
    echo "         - ensure target/platform/${1}/Dockerimagename names a suitable image" >&2
    echo "         - then use 'make all_docker'" >&2
    echo "       Delete the Vagrantfile to clear this error." >&2
    exit 1
else
    :
    #echo "NO Vagrant VM image found for this target"
    # This script doesn't do much for a build-hosted cross-compilation toolchain
    #echo "TARGET=$1" > ./TARGET.cfg // already done..
fi

exit 0
