#!/bin/bash
# P Drezet 2008

#This file gets other dependencies needed for EHS linking and for creating the install 


# Get VLC Repository with pre-built Binaries - this ma wnat to be a tagged version on the future
if test -e ../OTHER_DEPENDENCIES/VLC/$1; then 
svn cleanup ../OTHER_DEPENDENCIES/VLC/$1
svn update ../OTHER_DEPENDENCIES/VLC/$1
else
svn co svn://server/VLC/trunk/target_cs_packages/$1 ../OTHER_DEPENDENCIES/VLC/$1
fi 

if test -e ../OTHER_DEPENDENCIES/lua/
then 
	svn cleanup ../OTHER_DEPENDENCIES/lua
	svn update ../OTHER_DEPENDENCIES/lua
else
	svn co svn://server/LucidSource/branches/INX/environment/lua ../OTHER_DEPENDENCIES/lua
	#@todo this should be moved to OTHER DEPENDENCIES
fi  