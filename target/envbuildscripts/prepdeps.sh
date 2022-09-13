#!/bin/bash
# copyright inx limited UK 2010

# This script retrieves all support components built for the target type defined by the $OS and $ARCH
# This includes installing base host tools and the inxware dependency repos.
# This script should only be called via make prepdeps to setup the appropriate encironment variables.

set -e

if [ ! -n ${EHS_GNU_OS} ];then 
	echo Environment variable EHS_GNU_OS is not set - exiting.
	exit
fi

if [ ! -n ${EHS_GNU_ARCH} ];then 
	echo Environment variable ARCH is not set - exiting.
	exit
fi
# Check we have git lfs installed
git lfs env &>/dev/null || sudo apt-get install git-lfs

#Install GNU make and some basic host building tools.
if  ! command -v make &> /dev/null ; then 
  echo "Installing build-essentials for GNU make.  You will be asked for you user password for sudo " 
  sudo apt-get update
  sudo apt-get install build-essential
fi

#Check for curl we often want this and need it for the docker install
if  ! command -v curl &> /dev/null ; then 
  echo "Installing curl.... You may be asked for you user password for sudo " 
  sudo apt-get update
  sudo apt-get install curl
fi

# Check if we have socker installed and isntall it if we don't
if  ! command -v docker &> /dev/null ; then 
   echo "Docker not found. Installing... (You may be asked to enter your user password for sudo operations)" 
   sudo apt update
   sudo apt install apt-transport-https ca-certificates curl software-properties-common
   curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
   echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
   sudo apt update
   apt-cache policy docker-ce # check this is coming from the docker package manager not Ubuntu
   sudo apt install docker-ce
   sudo systemctl status docker
   sudo usermod -aG docker ${USER}
   read -n 1 "Docker has been installed, which requires you to reboot your machine or re-login so that new user permissions can take affect for ${USER}"
else 
   echo "Docker Already installed installed" 
fi

# Check we have git lfs installed
git lfs env &>/dev/null || sudo apt-get install git-lfs

# Set up some hardwired paramters for the inxware dependency repos
LOCAL_BASE="../"
REPOSITORY_BASE="ssh://git@dgithub.com:/inxware/"
#
# CORE PLATFORM SUPPORT FOR ERT-COMPONENTS & KERNEL BUILD
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
EHS_CORE_SUPPORT_REPO="EHS-build-support.git"

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
if [ ! -n "${SKIP_REPOS}" ];then
    echo "Retrieving latest core target support build directory for ${EHS_GNU_OS_ARCH} from ${EHS_CORE_SUPPORT_SERVER_PATH}"
    if [ -e $EHS_CORE_SUPPORT_LOCAL_PATH ]; then 
        echo "Found existing repo. Updating..."
        pushd $EHS_CORE_SUPPORT_LOCAL_PATH
        git pull
        git lfs pull
        popd
    else
        echo "No local repo found. Cloning the ert-build-support repository $EHS_CORE_SUPPORT_SERVER_PATH to $EHS_CORE_SUPPORT_LOCAL_PATH"
        git clone $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
    fi
    echo "Retrieving latest component support support build directory for ${EHS_GNU_OS_ARCH} from ${EHS_COMPONENT_SUPPORT_SERVER_PATH} ..."
    if [ -e $EHS_COMPONENT_SUPPORT_LOCAL_PATH ]; then 
        echo "Found existing Repo. Updating..."
        pushd $EHS_COMPONENT_SUPPORT_LOCAL_PATH
        git pull
        git lfs pull
        popd
    else
        echo "No local repo found. Cloning the ert-contrib-middleware repository $EHS_COMPONENT_SUPPORT_SERVER_PATH to $EHS_COMPONENT_SUPPORT_LOCAL_PATH"
        git clone $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
    fi
    echo "**********************************************************************"
    echo "Successfully retrieved dependencies for $EHS_GNU_OS_$EHS_GNU_ARCH"
    echo "**********************************************************************"
else 
    echo "Skipped dependency repo pull (SKIP_REPOS was set)"
fi

#todo2022 move this to a new make target like make all_docker
# First see if we need to run the VM.
# if we do then we will just open a vagrant VM and plonk you on a command line to run make etc. on your own!
# for now we will check for Docker run files to decide if we run docker, but might decide based on a config.mor target.mk k f in the future 
if  [ -f ${PWD}/target/platform/${1}/Vagrantfile ] ; then 
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
            #This shell will halt here  while the make commands are called
            vagrant halt
            popd
        fi #end INX_SKIP_VAGRANT
    fi #end NO_VM
else
    :
    #echo "NO Vagrant VM image found for this target"
    # This script doesnt do much for a  build-hosted cross-compilation toolchain
    #echo "TARGET=$1" > ./TARGET.cfg // already done..
fi
