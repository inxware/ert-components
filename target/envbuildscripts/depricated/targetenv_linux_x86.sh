#!/bin/bash

#Check that dependencies are present
echo SPECIFIC TARGERT = $2
if [ -n "$2" ] 
then # the target envi name is the same as the source from prepdeps.
SPECIFIC_TARGET=$2;
echo Creating Target pathto specific PATH =$SPECIFIC_TARGET
else 
SPECIFIC_TARGET=$1;
echo Creating Target path same as source target path =$SPECIFIC_TARGET
fi


#this target uses the caonical directory structure - so make it first
./target/envbuildscripts/targetenv_directories.sh $1 $2

svn --force export ../EHS/target/envtree/linux-ehs-tree/root-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
svn --force export ./target/envtree/linux-ehs-tree/devman/core-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/
svn --force export ./target/envtree/linux-ehs-tree/devman/plugins/player-ehs_dir/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/

# copy the ehs exeutable to the target
if test -e ehs_$SPECIFIC_TARGET; then 
	printf "ehs_"$SPECIFIC_TARGET" present - OK\n"; 
	cp ehs_$SPECIFIC_TARGET ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/ehs.exe
else 
	echo ehs_$SPECIFIC_TARGET not present - please build using make TARGET=$SPECIFIC_TARGET - Exiting ... 
exit
fi


# Add Lua - always!

./target/envbuildscripts/add_lua.sh $1 $SPECIFIC_TARGET
# the following is not needed: ./target/envbuildscripts/add_lua_$1.sh $1 $SPECIFIC_TARGET

echo making all bin files executable
chmod -R a+rwx ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
echo Done



exit
