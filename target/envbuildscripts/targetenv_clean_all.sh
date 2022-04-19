#!/bin/bash
# P Drezet 2008

#stop any crontabs on this machine that will create guff. Too brutal? ... Sorry.

if [[ ! -n ${1} ]]
then
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo No target path is defined ${1} - exiting
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
exit
fi

SPECIFIC_TARGET=$1;
#echo Creating Target path same as source target path =$SPECIFIC_TARGET

# remove any logs from debug versions etc.
rm -Rf ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET
