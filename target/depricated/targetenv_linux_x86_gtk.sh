#!/bin/bash
# P Drezet 2008

#Check that dependencies are present
echo Building target $1 : $2
if [ -n "$2" ] 
then # the target env name is the same as the source from prepdeps.
SPECIFIC_TARGET=$2;
#echo Creating Target pathto specific PATH =$SPECIFIC_TARGET
else 
SPECIFIC_TARGET=$1;
#echo Creating Target path same as source target path =$SPECIFIC_TARGET
fi

#Do the base stuff:
./target/envbuildscripts/targetenv_linux_x86.sh $1 $2


echo Copying VLC binaries to target tree at: ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
svn --force export ../OTHER_DEPENDENCIES/VLC/$1/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
echo Done




