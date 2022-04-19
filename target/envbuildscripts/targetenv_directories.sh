#!/bin/bash
# copyright inx limited 2010
# This populates the target tree with directories and common utilities.
# This script will populate OS specific core utilities and those common to all ARCHS.
# It does not populate the the runtime libraries 

export SPECIFIC_TARGET=$1;

#echo Creating Target path same as source target path =$SPECIFIC_TARGET

export TARGET_ENV_NAME="ehs_env-$SPECIFIC_TARGET"
export STAGING_DIRECTORY="../TARGET_TREES/$TARGET_ENV_NAME"
########################################################################

########################################################################
# Create a base directory structure suitable for all targets
echo "## CREATING GENERIC EHS FILE SYSTEM - Targetenv_directories.sh ###"
# create base directory if necessary
mkdir -p ../TARGET_TREES
echo NOT Removing any previous environment: $TARGET_ENV_NAME
#rm -f -r ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
echo Creating directories for $TARGET_ENV_NAME
mkdir -p $STAGING_DIRECTORY

#########################################################################
# Copy some standard files into the directories from various locations and repos.
# The following only applies to linux/unix but do it for all (lazy!)
#@todo Use $STAGING_DIRECTORY instead of the recompiled path in al the following

mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
cp -PR ../EHS/target/envtree/Generic-ehs-tree/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit

echo "#~~~~~~~~~~ Adding Genric Devman Components to EHS File System  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#"
#add the device management scripts
# @todo the following should check that the DEVMAN environment arguement has been passed.
mkdir -p ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/devman
cp -PR ./target/envtree/Generic-ehs-tree/devman/core-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit
cp -PR ./target/envtree/Generic-ehs-tree/devman/plugins/player-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit

# ./target/envbuildscripts/targetenv_clean_config.sh $SPECIFIC_TARGET

#########################################################################
# Copy the target specific files to the file system
# note - may not be target specific files for some OSs
test -e ../EHS/target/envtree/$EHS_OS-ehs-tree/root-ehs_dir && cp -PR ../EHS/target/envtree/$EHS_OS-ehs-tree/root-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit
test -e ../EHS/target/envtree/$EHS_OS-ehs-tree/devman/core-ehs_dir && cp -PR ../EHS/target/envtree/$EHS_OS-ehs-tree/devman/core-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit
test -e ../EHS/target/envtree/$EHS_OS-ehs-tree/devman/plugins/player-ehs_dir && cp -PR ../EHS/target/envtree/$EHS_OS-ehs-tree/devman/plugins/player-ehs_dir/* ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/ || exit


########################################################################
## Retrieve version info for the output..
read -r -d '*'  VERSION_NAME TIME_STAMP REST < $STAGING_DIRECTORY/sysdata/version.nfo 

echo "######################################################################################################"
echo "### Done Build:$VERSION_NAME - $TIME_STAMP"
echo "### Cleaned only Devman state and runtime logs, all applications and user data remains"
echo "### To completely clean the target tree delete the TARGETENV directory and re-run this make"
echo "######################################################################################################"


 
