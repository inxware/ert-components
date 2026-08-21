#!/system/bin/sh

# this is called from the ehs supervisor

InitAppManger(){
	SupervisorLog "(InitAppManger) Replace this script, with a system specific script e.g ambifier, sso etc."
}

RunAppManger(){
	SupervisorLog "(RunAppManger) Replace this script, with a system specific script e.g ambifier, sso etc."
}

RestartApp(){
	SupervisorLog "(RestartApp) Replace this script, with a system specific script e.g ambifier, sso etc."
}

DevicePrefix(){
	# Override this function to specify the prefix of the managed device ID.
}

AppEventsHandler(){
	# Override this function to handle any app specific events
}
