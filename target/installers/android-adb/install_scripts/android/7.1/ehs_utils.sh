#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils_common.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

StartService(){
	SERVICE=$1
	echo "Starting Service $SERVICE ..."
	am startservice $SERVICE
}

SetAsHomeApp(){
	PACKAGE=$1
	INTENT=$2
	if ! [ -z "$INTENT" ]; then
		echo "Setting ($PACKAGE) as home app ..."
		cmd package set-home-activity $PACKAGE/$INTENT
		LaunchHomeApp
	fi
}

GetHomeApp(){
	cmd shortcut get-default-launcher
}

RestartNetwork(){
	SupervisorLog "Restart network - start"

	if [ -z "$(ps | grep netd)" ]; then
		SupervisorLog "netd daemon is NOT running"
		start netd
		sleep 4
		if [ -z "$(ps | grep netd)" ]; then
			SupervisorLog "netd daemon OK"
		else
			SupervisorLog "Failed to start netd daemon"
		fi
	fi

	ifconfig eth0 down
	SupervisorLog "ifconfig eth0 down"
	sleep 2
	ifconfig eth0 up
	SupervisorLog "ifconfig eth0 up"

	SupervisorLog "Restart network - done"
}