
COMLINE="${0}"
COMLINECHAR1="${COMLINE:(0):(1)}"
if [ "${COMLINECHAR1}" = "/" ]; then
  DIRECTORY="`dirname $0`"
else
  DIRECTORY="${PWD}/`dirname $0`"
fi
export INXWAREROOT="${DIRECTORY}/../"

$DIRECTORY/stop_ehs.sh