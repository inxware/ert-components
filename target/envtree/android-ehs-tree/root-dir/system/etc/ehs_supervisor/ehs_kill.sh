#!/system/bin/sh

EHS_SUPERVISOR_LOCATION="/system/etc/ehs_supervisor"

source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"

SERVICE=$(RunningProcess "ehs_service")

if ! [ -z "$SERVICE" ]; then
	kill -9 $SERVICE
fi

SUPERVISOR=$(RunningProcess "ehs_supervisor")

if ! [ -z "$SUPERVISOR" ]; then
	kill -9 $SUPERVISOR
fi

EVENT_HANDLER=$(RunningProcess "ehs_event_handler")

if ! [ -z "$EVENT_HANDLER" ]; then
	kill -9 $EVENT_HANDLER
fi