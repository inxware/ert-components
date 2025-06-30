#!/bin/bash
# copyright inx limited 2010
# This populates the target tree with directories and common utilities.
# This script will populate OS specific core utilities and those common to all ARCHS.
# It does not populate the the runtime libraries 
set -e
export SPECIFIC_TARGET=$1;
export TARGET_ENV_NAME="ehs_env-$SPECIFIC_TARGET"
export STAGING_DIRECTORY="../TARGET_TREES/$TARGET_ENV_NAME"
########################################################################

########################################################################
# Create a base directory structure suitable for all targets
echo "############# Creating Generic eRT file system (targetenv_directories.sh)  #########################"
# create base directory if necessary
mkdir -p ../TARGET_TREES
echo "NOT Removing any previous environment: ${TARGET_ENV_NAME}"
#rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
echo "Creating directories for ${TARGET_ENV_NAME}"
mkdir -p $STAGING_DIRECTORY

#########################################################################
# Copy some standard files into the directories from various locations and repos.
# The following only applies to linux/unix but do it for all (lazy!)
#@todo Use $STAGING_DIRECTORY instead of the recompiled path in al the following

mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
cp -PR ../ert-components/target/envtree/Generic-ehs-tree/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/  


if [ "${EHS_DEVMAN_SUPERVISOR_REQUIRED}" = "yes" ]; then
   echo -e "++++++++++++++++++++ Adding Generic Devman Components to ert file system   \t++++++++++++++++++++"
   mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman
   cp -PR ./target/envtree/Generic-ehs-tree/devman/core-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ 
   cp -PR ./target/envtree/Generic-ehs-tree/devman/plugins/player-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ 
fi

# ./target/envbuildscripts/targetenv_clean_config.sh $SPECIFIC_TARGET
echo -e "++++++++++++++++++++ Adding OS & ARCH specific components to ert file system  \t++++++++++++++++++++"
# Copy the target specific files to the file system
# note - may not be target specific files for some OSs
test -e ../ert-components/target/envtree/$EHS_OS-ehs-tree/root-ehs_dir && cp -PR ../ert-components/target/envtree/$EHS_OS-ehs-tree/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || echo "!!Not copying: ./$EHS_OS-ehs-tree/root-ehs_dir"
if [ "${EHS_DEVMAN_SUPERVISOR_REQUIRED}" = "yes" ]; then
   test -e ../ert-components/target/envtree/$EHS_OS-ehs-tree/devman/core-ehs_dir && cp -PR ../ert-components/target/envtree/$EHS_OS-ehs-tree/devman/core-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || echo "!!Not copying: ./$EHS_OS-ehs-tree/devman/core-ehs_dir"
   test -e ../ert-components/target/envtree/$EHS_OS-ehs-tree/devman/plugins/player-ehs_dir && cp -PR ../ert-components/target/envtree/$EHS_OS-ehs-tree/devman/plugins/player-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || echo "!!Not copying: ./$EHS_OS-ehs-tree/devman/plugins/player-ehs_dir"
fi
########################################################################
## Adding more paltform and product specific asssets and scripts: 

# Some devices have varying OSs features that need to be modified:
if  test -d ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}/root-ehs_dir
then
   echo -e "++++++++++++++++++++ Applying ${SYSTEM_VARIANT} assets to staging directory \t++++++++++++++++++++"
	cp -PR ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
else
   echo -e "-------------------" NOT Applying ${SYSTEM_VARIANT} assets to staging directory -------------------""
fi

# Some devices have varying OSs features that need to be added.
# NOTE: These will clobber any system_variant versions of the same files!
if [ "${PRODUCT_VARIANT}" != ""  ]; then
   if  test -d ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}-${PRODUCT_VARIANT}/root-ehs_dir
   then
      echo -e "++++++++++++++++++++ Applying [${PRODUCT_NAME}] assets for SYSTEM VARIANT to staging directory  \t\t++++++++++++++++++++"
      cp -PR ./target/envtree/PLATFORM-SPECIFIC/${SYSTEM_VARIANT}-${PRODUCT_VARIANT}/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
   else
      echo -e "------------------- NOT Applying [${PRODUCT_NAME}] assets for SYSTEM VARIANT"
   fi
else 
      echo -e "-------------------- PRODUCT_VARIANT variable is not set."
fi

#### Add selected Host OS configuration scripts that should run at install (first install boot more precisely) ####
echo "++++++++++++++++++++ Adding Host OS configuration sripts                        ++++++++++++++++++++"
echo "Selected scripts:"
echo "${HOST_OS_CONFIG_SCRIPTS}"
#Create the script directory if it isn't in the env
mkdir -p ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit
#remove any directory place holders or residual runtime spam
rm -f ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit/*
for i in ${HOST_OS_CONFIG_SCRIPTS}; do 
	cp ./target/envtree/${EHS_OS}-ehs-tree/system/HostOsInit/$i.* ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/bin/HostOsInit/
done
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

########################################################################
## Retrieve version info for the output..
if [ -e "${STAGING_DIRECTORY}/sysdata/version.nfo" ];then 
   read -r -d '*'  VERSION_NAME TIME_STAMP REST < ${STAGING_DIRECTORY}/sysdata/version.nfo ||:
else
   
   echo "Warning !!!!!! No version information file is present for this build. Setting to UNKNOWN. !!!!!!!!"
   echo "Run make targetenv_version to generate version string for this release"
   VERSION_NAME="UNKOWN"
   TIME_STAMP="UNKOWN"
fi
echo "#####################################################################################################"
echo "### Done Staging Directory Build for version :$VERSION_NAME - $TIME_STAMP"
echo "### Cleaned only Devman state and runtime logs, all applications and user data remains"
echo "### To completely clean the target tree delete the TARGETENV directory and re-run this make"
echo "####============================ DONE TAREGETENV_DIRECTORIES ===================================#####"
echo "#####################################################################################################"

