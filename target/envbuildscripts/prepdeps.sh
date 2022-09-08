#!/bin/bash
# copyright inx limited UK 2010

# This script retrieves all support components built for the target type defined by the $OS and $ARCH

set -e

#echo "Retrieving core target  support build directory for $EHS_OS_$ARCH from $EHS_COMPONENT_SUPPORT_LOCAL_PATH"

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


LOCAL_BASE="../"
REPOSITORY_BASE="ssh://tech-data@dev.inx-systems.net:8822/home/inx-data/data/Repos/"

#
# CORE PLATFORM SUPPORT FOR EHS
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
#TODO2022 - rename these on the srver as follows: EHS_CORE_SUPPORT_REPO="${EHS_CORE_SUPPORT_DIR}.git"
EHS_CORE_SUPPORT_REPO="EHS-build-support.git"

#EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/"
if [ ! -n ${EHS_CLIB_OVERRIDE_PATH} ];then 
	EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/$(EHS_GNU_OS_ARCH)${EHS_SPECIAL_CLIB_EXT}"
else
	EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/${EHS_CLIB_OVERRIDE_PATH}"
fi

# The following contains toolcains, kernel headers and other clibs
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
echo "SKIP REPOS PULL = ${SKIP_REPOS}"
if [ ! -n "${SKIP_REPOS}" ];then


#echo "Retrieving core target  support build directory for ${EHS_GNU_OS_ARCH} from ${EHS_COMPONENT_SUPPORT_LOCAL_PATH}"

if [ -e $EHS_CORE_SUPPORT_LOCAL_PATH ]; then 
	echo updating ert-build-support repository to $EHS_CORE_SUPPORT_LOCAL_PATH
	pushd $EHS_CORE_SUPPORT_LOCAL_PATH
	git pull
	git lfs pull
	popd
	#svn cleanup $EHS_CORE_SUPPORT_LOCAL_PATH
	#svn update $EHS_CORE_SUPPORT_LOCAL_PATH
else
	echo Cloning the ert-build-support repository $EHS_CORE_SUPPORT_SERVER_PATH to $EHS_CORE_SUPPORT_LOCAL_PATH
	git clone $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
	#svn co $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
fi

echo "Retrieving component support support build directory for ${EHS_GNU_OS_ARCH}"
if [ -e $EHS_COMPONENT_SUPPORT_LOCAL_PATH ]; then 
	echo updating ert-contrib-middleware library repository to $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	pushd $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	git pull
	git lfs pull
	popd
	#svn cleanup $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	#svn update $EHS_COMPONENT_SUPPORT_LOCAL_PATH
else
	echo Cloning the ert-contrib-middleware  repository $EHS_COMPONENT_SUPPORT_SERVER_PATH to $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	git clone $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	#svn co $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
fi

echo "*************************************"
echo "Done Retrieved dependencies for  $EHS_GNU_OS_$EHS_GNU_ARCH"
echo "*************************************"
fi


# First see if we need to run qa VM
#Note we may move this later in the  script and find the VMS in the relevant os-arch directory instead
#for now we will check for Docker run files to decide if we run docker, but might decide based on a config.mor target.mk k f in the future 
if  [ -f ${PWD}/target/platform/${1}/Vagrantfile ] ; then 
    if [ "$2" == "NO_VM"  ]; then
        echo "Not doing Vagrant VM because arg#2 = NO_VM, checking for Docker "
    else
        if [ "${INX_SKIP_VAGRANT}" == "true" ]; then
            echo "skipping vagrant due to INX_SKIP_VAGRANT set in the environment"
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
    echo "NO Vagrant VM, checkking for Docker "
    # This script doesnt do much for a  build-hosted cross-compilation toolchain
    #echo "TARGET=$1" > ./TARGET.cfg // already done..
fi
