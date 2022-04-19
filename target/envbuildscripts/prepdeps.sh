#!/bin/bash
# copyright inx limited UK 2010

# This script retrieves all support components built for the target type defined by the $OS and $ARCH

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
	REPOSITORY_BASE="ssh://repo-user@server:/home/inx-data/data/Repos/"
fi

#
# CORE PLATFORM SUPPORT FOR EHS
#
EHS_CORE_SUPPORT_DIR="ert-build-support"
EHS_CORE_SUPPORT_REPO="${EHS_CORE_SUPPORT_DIR}.git"

#EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/"
if [ ! -n ${EHS_CLIB_OVERRIDE_PATH} ];then 
	EHS_CORE_SUPPORT_PATH_FROM_BASE="${EHS_CORE_SUPPORT_DIR}/target_libs/${EHS_GNU_ARCH}-${EHS_GNU_OS}-${EHS_GNU_OS_VERSION}${EHS_SPECIAL_CLIB_EXT}"
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



#echo "Retrieving core target  support build directory for $EHS_OS_$ARCH from $EHS_COMPONENT_SUPPORT_LOCAL_PATH"

if [ -e $EHS_CORE_SUPPORT_LOCAL_PATH ]; then 
	echo updating EHS build support repository to $EHS_CORE_SUPPORT_LOCAL_PATH
	pushd $EHS_CORE_SUPPORT_LOCAL_PATH
	git pull
	popd
	#svn cleanup $EHS_CORE_SUPPORT_LOCAL_PATH
	#svn update $EHS_CORE_SUPPORT_LOCAL_PATH
else
	echo Cloning the EHS build support repository $EHS_CORE_SUPPORT_SERVER_PATH to $EHS_CORE_SUPPORT_LOCAL_PATH
	git clone $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
	#svn co $EHS_CORE_SUPPORT_SERVER_PATH $EHS_CORE_SUPPORT_LOCAL_PATH
fi

echo "Retrieving component support support build directory for $EHS_OS_$ARCH"
if [ -e $EHS_COMPONENT_SUPPORT_LOCAL_PATH ]; then 
	echo updating component library repository to $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	pushd $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	git pull
	popd
	#svn cleanup $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	#svn update $EHS_COMPONENT_SUPPORT_LOCAL_PATH
else
	echo Cloning the component library repository $EHS_COMPONENT_SUPPORT_SERVER_PATH to $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	git clone $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
	#svn co $EHS_COMPONENT_SUPPORT_SERVER_PATH $EHS_COMPONENT_SUPPORT_LOCAL_PATH
fi

echo "*************************************"
echo "Done Retrieved dependencies for  $EHS_GNU_OS_$EHS_GNU_ARCH"
echo "*************************************"



