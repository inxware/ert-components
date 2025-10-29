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

# Check we have git lfs installed
echo "Checking git lfs is installed. You may be asked for you user password for sudo"
git lfs env &>/dev/null || sudo apt -y install git-lfs

#Install GNU make and some basic host building tools.
if ! command -v make &> /dev/null ; then
    echo "Installing build-essentials for GNU make. You may be asked for you user password for sudo"
    sudo apt update -qq
    sudo apt -y install build-essential
fi

#Check for curl we often want this and need it for the docker install
if ! command -v curl &> /dev/null ; then
    echo "Installing curl. You may be asked for you user password for sudo"
    sudo apt update -qq
    sudo apt -y install curl
fi

# Check if we have socker installed and isntall it if we don't
if ! command -v docker &> /dev/null ; then
    echo "Docker not found. Installing. (You may be asked to enter your user password for sudo operations)"
    sudo apt update -qq
    sudo apt -y install apt-transport-https ca-certificates curl software-properties-common
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
    sudo apt update -qq
    apt-cache policy docker-ce # check this is coming from the docker package manager not Ubuntu
    sudo apt -y install docker-ce
    sudo systemctl status docker
    sudo usermod -aG docker ${USER}
    read -n 1 -p "${TXT_FG_RED}Docker has been installed, which requires you to reboot your machine or re-login so that new user permissions can take affect for ${USER}"
else
    echo "Docker Already installed installed"
fi

# Check if we have docker-compose installed and install it if we don't
warn "We're not checking the python environment for esp32 tools... find this line and enable if you want to know how..."
if [ 1 = 0 ];then
    # Check we have git lfs installed
    if ! command -v pip &> /dev/null ; then
        # Check we have python and pip
        sudo apt -y install python3 python3-pip python3-venv
        pip install pyserial
        #export IDF_PYTHON_ENV_BASE="../../TARGET_TREES/esp32_venv"
        #python3 -m venv "../TARGET_TREES/esp32_venv" > /dev/null - WE SEEM TO BE USING /opt these days.
        python3 -m venv "/opt/python_env" > /dev/null
        pip install -r "../ert-contrib-middleware/inx_build_scripts/source-scripts/python-pip-requirements_inx-xbuilder-source-me-espidf.txt" > /dev/null
    else
        echo "Python already installed."
    fi
fi

# Set up some hardwired paramters for the inxware dependency repos
# TODO2025: THIS SHOULD BE A BASE MAKE SYSTEM VARIABLE THAT IS EXPORTED TO HERE AND OTHER SCRIPTS
LOCAL_BASE="../"

if [ "$IS_PUBLIC" = "yes" ]; then
    REPOSITORY_BASE="$(dirname -- ${REPO_URL})"
else
    REPOSITORY_BASE="ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/"
fi

#
# CORE PLATFORM SUPPORT FOR ERT-COMPONENTS & KERNEL BUILD
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
EHS_CORE_SUPPORT_REPO="ert-build-support.git"

if [ ! -n ${EHS_CLIB_OVERRIDE_PATH} ];then
    EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/$(EHS_GNU_OS_ARCH)${EHS_SPECIAL_CLIB_EXT}"
else
    EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/${EHS_CLIB_OVERRIDE_PATH}"
fi

# The following paths contain toolcains, kernel headers and other c-abi libraries.
EHS_CORE_SUPPORT_SERVER_PATH="${REPOSITORY_BASE}/${EHS_CORE_SUPPORT_REPO}"
EHS_CORE_SUPPORT_LOCAL_PATH="${LOCAL_BASE}/${EHS_CORE_SUPPORT_DIR}"
EHS_TOOLCHAIN_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/toolchains/${TOOLCHAIN_PATH}"
EHS_TOOLCHAIN_LOCAL_PATH="${LOCAL_BASE}/${EHS_TOOLCHAIN_PATH_FROM_BASE}"

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
EHS_COMPOENT_SUPPORT_REPO="comp-lib-support.git"
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
        git clone $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
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
        git clone $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
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
            git clone $EHS_KERNEL_SERVER_PATH $EHS_KERNEL_LOCAL_PATH
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

#todo2022 move this to a new make target like make all_docker
# First see if we need to run the VM.
# if we do then we will just open a vagrant VM and plonk you on a command line to run make etc. on your own!
# for now we will check for Docker run files to decide if we run docker, but might decide based on a config.mor target.mk k f in the future
if  [ -f ${PWD}/target/platform/${1}/Vagrantfile ]; then
    if [ "$2" == "NO_VM"  ]; then
        echo "Not doing Vagrant VM because arg#2 = NO_VM, checking for Docker "
    else
        if [ "${INX_SKIP_VAGRANT}" == "true" ]; then
            echo "Skipping vagrant due to INX_SKIP_VAGRANT set in the environment"
        else
            echo "!!! Ths platform build requires a vm to be started by vagrant!!!"
            echo " If you continue this shell will be restarted in a VM, where the build will take place"
            echo "You will still work from this current directory from inside the VM and will be able to use the make targets as usual:"
            echo "Press ctr-C to exit or any other key to continue"
            read -n 1
            
            VAGRANT_STAGING_DIR="${PWD}/../TARGET_TREES/VAGRANT/cachespace"
            mkdir -p  ${VAGRANT_STAGING_DIR} || exit
            cp  ./target/platform/$1/Vagrantfile ${VAGRANT_STAGING_DIR}/|| exit
            pushd ${VAGRANT_STAGING_DIR}
            vagrant up
            vagrant ssh -- -t 'cd /vagrant_data/ert-components && pwd ; /bin/bash'
            #This shell will halt here while the make commands are called
            vagrant halt
            popd
        fi #end INX_SKIP_VAGRANT
    fi #end NO_VM
else
    :
    #echo "NO Vagrant VM image found for this target"
    # This script doesn't do much for a build-hosted cross-compilation toolchain
    #echo "TARGET=$1" > ./TARGET.cfg // already done..
fi

exit 0
