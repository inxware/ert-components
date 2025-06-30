#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils_common.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

StartService(){
	SERVICE=$1
	echo "Starting Service $SERVICE ..."
	am start-foreground-service $SERVICE
}

SetAsHomeApp(){
	PACKAGE=$1
	INTENT=$2
	if ! [ -z "$INTENT" ]; then
		echo "Setting ($PACKAGE) as home app ..."
		pm set-home-activity $PACKAGE/$INTENT
		LaunchHomeApp
	fi
}

GetHomeApp(){
	dumpsys activity | grep mHomeProcess
}

RestartNetwork(){
	SupervisorLog "RestartNetwork - not implemented for android 9.0"
}