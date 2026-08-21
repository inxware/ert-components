#!/bin/bash
#####################################################################
# This is a utility for the CI system that migth want to copy files to 
# target machines on the test LAN
# TODO - THIS IS MOST LIKELY DUPLICATED BY SCRIPTS IN ./scrips/build-deploy/
#####################################################################



if [  "$1" == "" ]; then
	echo "Please provide a path to the files to deploy"
	exit
else 
	files=$1
fi

if [  "$2" == "" ]; then
	echo "Please provide a path to the target path to deploy to"
	exit
else 
	targetpath=$2
fi

if [  "$3" == "" ]; then
	echo "Please provide a target IP address or URL for ssh"
	exit
else 
	url=$3
fi


if [ "$4" == "" ];then
	username=root
else
	username=$4
fi

if [ "$5" == "" ];then
	password=""
	passwd_prefix_command=""
else
	password=$5
	apt install sshpass
	passwd_prefix_command=sshpass -p ${password}

	# or use expect??	
fi

if [ "$6" == "" ];then
	port=22
else
	port=$6
fi

${passwd_prefix_command} scp -P ${port} ${files} ${username}@${url}:${targetpath}
