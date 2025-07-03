#!/bin/bash
# P Drezet 2008

echo build machine package requirements:
echo 'gcc (latest version ?)'
echo 'subversion (apt-get)'
echo 'pkg-config (apt-get)'
echo 'libgtk2.0-dev'
echo 'Check for corret version of libc.so.0.6 ( 2.5 or 2.7) if run error '

#@todo this should be sourcing the general linux x86 (no gtk script).

#This file gets other dependencies needed for EHS linking and for creating the install 
VLCVERSION=Version_1.1.2
#VLCVERSION=INX
#VLCVERSION="../trunk" this doesn't work! We will always use branches in any case!


#Get USBLCD libraries
if test -e ../OTHER_DEPENDENCIES/USBLCD/$1
then
	svn cleanup ../OTHER_DEPENDENCIES/USBLCD/$1
	svn update ../OTHER_DEPENDENCIES/USBLCD/$1
else
	svn co svn://server/LinuxSupport/trunk/PicoLCD/linux_x86_gtk ../OTHER_DEPENDENCIES/USBLCD/$1
fi

echo Getting VLC version $VLCVERSION runtime libraries
if test -e ../OTHER_DEPENDENCIES/VLC/$1
then 
	svn cleanup ../OTHER_DEPENDENCIES/VLC/$1
	svn update ../OTHER_DEPENDENCIES/VLC/$1
else
	svn co svn://server/VLC/branches/$VLCVERSION/target_cs_packages/linux_x86_gtk ../OTHER_DEPENDENCIES/VLC/$1
fi


echo Done getting dependencies for $1
