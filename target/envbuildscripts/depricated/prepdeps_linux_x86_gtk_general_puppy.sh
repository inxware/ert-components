#!/bin/bash
# P Drezet 2008

#Get extra libraries that puppy linux needs 

if test -e ../OTHER_DEPENDENCIES/Libs/$1
then 
	svn cleanup ../OTHER_DEPENDENCIES/Libs/$1
	svn update ../OTHER_DEPENDENCIES/Libs/$1
else
	svn co svn://server/LinuxSupport/trunk/Libs/linux_x86_gtk ../OTHER_DEPENDENCIES/Libs/$1
fi

#the rest is generic linux gtk and standard VLC stuff - pass on the specific target ame for path purposes

./target/envbuildscripts/prepdeps_linux_x86_gtk.sh $1 $2
