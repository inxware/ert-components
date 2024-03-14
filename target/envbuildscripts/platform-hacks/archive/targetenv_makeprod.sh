#!/bin/bash
#
# This script configures the EHS tree with standard INX produts (e.g.Brix) with appropriate apps and paths:
#  Standard home app
#  A graphics configuration file
#  The INX devman server
#
# It removes any development crud from the target tree.
# It can be used as a base productionisation script and maybe hacked by later 

#cleanup the directories
SPECIFIC_TARGET=$1

./target/envbuildscripts/targetenv_clean_config.sh $TARGET

#Install the default apps 

mkdir -p ../TARGET_TREES/ehs_env-$TARGET/appdata/default
rm -f ../TARGET_TREES/ehs_env-$TARGET/appdata/default/*
cp -Rf ../apps/systemapps/Home/export/* ../TARGET_TREES/ehs_env-$TARGET/appdata/default/
#@todo ths shuld be conditional on desktop wndowing environment

mkdir -p ../TARGET_TREES/ehs_env-$TARGET/sysdata
cp ./target/envtree/Generic-ehs-tree/system/graphics.nfo ../TARGET_TREES/ehs_env-$TARGET/sysdata/

#configure the devman URLs:
echo "Adding standard inx Devman URLS"
echo "https://devman.inx-systems.com" > ../TARGET_TREES/ehs_env-$TARGET/devman/core/config/DEVMANURL.000
#todo shouldn't we actually use this as a default now in the c code?
#echo "http://devman.inx-systems.com/devman_player.php" > ../TARGET_TREES/ehs_env-$TARGET/devman/plugins/1/DEVMANPLAYERURL.000
