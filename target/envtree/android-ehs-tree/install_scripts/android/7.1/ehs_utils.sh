#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_utils_common.sh"

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