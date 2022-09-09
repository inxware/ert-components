#!/system/bin/sh

LOG_PREFIX="[EHSSUPERVISOR]"

SupervisorLog(){
	MSG=$1
	log "$LOG_PREFIX $MSG"
}

SupervisorError(){
	MSG=$1
	log -p e "$LOG_PREFIX $MSG"
}