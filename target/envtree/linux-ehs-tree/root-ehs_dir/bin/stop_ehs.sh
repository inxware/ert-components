#!/bin/bash

COMLINE="${0}"
COMLINECHAR1="${COMLINE:(0):(1)}"
if [ "${COMLINECHAR1}" = "/" ]; then
  DIRECTORY="`dirname $0`"
else
  DIRECTORY="${PWD}/`dirname $0`"
fi
export INXWAREROOT="${DIRECTORY}/../"

touch "${INXWAREROOT}/sysdata/stop_all.flag"
"${DIRECTORY}/restart.sh"
COUNTER=0
while [ -e "${INXWAREROOT}/sysdata/stop_all.flag" ]  
do
	sleep 1
	let COUNTER=COUNTER+1
 	test $COUNTER -gt 10  &&  break			
done
#Last resort nuke all you can anyway you can
if test $COUNTER -gt 10 
then
	killall ehs.exe
	killall run_ehs.sh
	"${DIRECTORY}/restart.sh"
fi
rm -f "${INXWAREROOT}/sysdata/stop_all.flag"

