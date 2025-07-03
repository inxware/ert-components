#!/bin/bash
# P Drezet 2008

#This file gets other dependencies needed for EHS linking and for creating the install 



# Get VLC Repository with pre-built Binaries - this ma wnat to be a tagged version on the future
#pwd
#exit 1
if test -e ../OTHER_DEPENDENCIES/VLC/$1
then 
	svn cleanup ../OTHER_DEPENDENCIES/VLC/$1
	svn update ../OTHER_DEPENDENCIES/VLC/$1
else
	svn co svn://server/VLC/trunk/target_cs_packages/$1 ../OTHER_DEPENDENCIES/VLC/$1
fi 

./target/envbuildscripts/prepdeps_linux_x86_gtk.sh $1