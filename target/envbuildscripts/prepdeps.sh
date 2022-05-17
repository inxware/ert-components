#!/bin/bash
# copyright inx limited UK 2010

# This script retrieves all support components built for the target type defined by the $OS and $ARCH

#echo "Retrieving core target  support build directory for $EHS_OS_$ARCH from $EHS_COMPONENT_SUPPORT_LOCAL_PATH"

if [ ! -n ${EHS_GNU_OS} ];then 
	echo Environment variable EHS_GNU_OS is not set - exiting.
	exit
fi


if [ ! -n ${EHS_GNU_ARCH} ];then 
	echo Environment variable ARCH is not set - exiting.
	exit
fi

LOCAL_BASE="../"

if [ ! -n "${REPOSITORY_BASE}" ]; then
	REPOSITORY_BASE="ssh://git@github.com:inxware/"
fi

#
# CORE PLATFORM SUPPORT FOR EHS
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
EHS_CORE_SUPPORT_REPO="${EHS_CORE_SUPPORT_DIR}.git"

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
#  obsolete EHS_KERNEL_HEADER_PATH_FROM_BASE="ert-build-support/kernel-dependencies/${KERNEL_VERSION}"
# obsolete EHS_KERNEL_HEADER_LOCAL_PATH="${LOCAL_BASE}/${EHS_KERNEL_HEADER_PATH_FROM_BASE}"


#
# COMPONENT SUPPORT LIBRARIES FOR PLATFORM
#
COMPONENT_LIB_SUPPORT_DIR="ert-contrib-middleware"
EHS_CORE_SUPPORT_REPO="${COMPONENT_LIB_SUPPORT_DIR}.git"

EHS_COMPONENT_SUPPORT_LOCAL_PATH="${LOCAL_BASE}/${COMPONENT_LIB_SUPPORT_DIR}/"
EHS_COMPONENT_SUPPORT_SERVER_PATH="${REPOSITORY_BASE}/${EHS_CORE_SUPPORT_REPO}"

#
# GET THE STUFF!! if specifics have been given 
#

if [ 1 == 1 ];then


#echo "Retrieving core target  support build directory for ${EHS_GNU_OS_ARCH} from ${EHS_COMPONENT_SUPPORT_LOCAL_PATH}"

if [ -e $EHS_CORE_SUPPORT_LOCAL_PATH ]; then 
	echo updating ert-build-support repository to $EHS_CORE_SUPPORT_LOCAL_PATH
	pushd $EHS_CORE_SUPPORT_LOCAL_PATH
	git pull
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

            VAGRANT_STAGING_DIR=".${PWD}/./TARGET_TREES/VAGRANT/cachespace"
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

# Check to see if we should use a docker image
#SUDO_COMMAND=sudo # tdo make this default off
SUDO_COMMAND= # Dot do this use the docker group instead:

#if newgrp docker ; then
#echo Switched to docker group OK
#else
echo "Running in Docker Container..."
echo "If you get access errors then you probably don't belong to the docker group - please fix this with"
echo -e "  \$sudo groupadd docker"
echo -e "  \$sudo usermod -aG docker \$USER"
echo -e "  \$newgrp docker"
#fi
PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${1}/Dockerimagename"
PATH_TO_TARGET_DOCKERFILE="${PWD}/target/platform/${1}/Dockerfile"
        
#todo check for a config.mk variable - either as an argument or set by mkae as an environment variable.
echo "|${PATH_TO_TARGET_DOCKER_IMAGE}|"
if [ -f  ${PATH_TO_TARGET_DOCKER_IMAGE} ]; then
  echo -n "Found Docker image... :"
    DOCKER_STAGING_DIR="${PWD}/../TARGET_TREES/DOCKER/cachespace"
    mkdir -p  ${DOCKER_STAGING_DIR} ||exit
    pushd ${DOCKER_STAGING_DIR} || exit
    read DOCKER_IMAGE <  ${PATH_TO_TARGET_DOCKER_IMAGE} || echo "Could not read ${PATH_TO_TARGET_DOCKER_IMAGE} "  
    echo " Setting up ${DOCKER_IMAGE}"
     ${SUDO_COMMAND} docker pull  ${DOCKER_IMAGE} 
    if ${SUDO_COMMAND} docker image inspect ${DOCKER_IMAGE}  &> /dev/null ; then
        echo "Using existing Docker image "
        echo "Current PWD = "$(pwd)
        ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm  --privileged -it \
             -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            sh -c "pwd && ls -l .. && make -j 8 && make targetenv"
        #sh -c "pwd && ls -al && ls -al ../"
    else 
       echo "Trying to build the dockerfile"
        #Try and build the image

# TODO -make the following work! 
 # Zuhaib to # 
 # (1) check if the cusom image name is already installed on the host - DONE see above!
 # (2) cd to the ../TARGET_TREE working directory - DONE see above!
 # (3) make the custome image from a Dockerfile that doe examplen includes the gtk and gstreamer dev library packages & install it (using build?)
 # (4) Checkk all good and move onto the next. 


        echo "Dockerfile path ="${PATH_TO_TARGET_DOCKERFILE} 
        if  [ -f  ${PATH_TO_TARGET_DOCKERFILE} ] ; then
            echo "found Dockerfile.."
           #Get the Image Name we want to run (and possibly build)
            #todo we should  check if the imagei is already available? Or needs a force update
            mkdir -p  ${DOCKER_STAGING_DIR} || exit
            ${SUDO_COMMAND} docker build  -f ${PATH_TO_TARGET_DOCKERFILE} -t  ${DOCKER_IMAGE}  .
          
             echo "Current PWD = "$(pwd)
            ${SUDO_COMMAND} docker run --user $(id -u):$(id -g) --rm --privileged -it \
             -v "$(pwd)/../../../:/inxware"  -w "/inxware/ert-components/"\
            ${DOCKER_IMAGE}\
            sh -c "pwd && ls -l .. &&  make -j 8 && make targetenv"
            fi
    fi
    popd
fi 


