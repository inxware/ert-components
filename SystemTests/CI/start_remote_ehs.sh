#!/bin/bash

if [  "$1" == "" ]; then
	echo "Please provide an IP address of the remote device on which to start ehs"
	exit
else 
	ipaddr=$1
fi

if [  "$2" == "" ]; then
	timeout_seconds=0
else 
	timeout_seconds=$2
fi


if [ "$3" == "" ];then
	username=root
else
	username=$3
fi

if [ "$4" == "" ];then
	password=""
	passwd_prefix_command=""
else
	password=$4
	apt install sshpass
	passwd_prefix_command=sshpass -p ${password}

	# or use expect??	
fi

if [ "$5" == "" ];then
	port=22
else
	port=$5
fi

if [ "$time_seconds" == "0" ]; then 
	ssh -p ${port} ${username}@${ipaddr} /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST
else
	ssh -p ${port} ${username}@${ipaddr} timeout ${ timeout_seconds} /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST
fi


