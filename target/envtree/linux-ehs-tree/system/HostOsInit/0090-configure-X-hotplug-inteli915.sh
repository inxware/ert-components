#!/bin/bash
# This script should be used on first boot only. It sets up the debian jessie deb repo when the 
# system first runs after a USB stick install
export PATH=$PATH:/usr/sbin
set +e
set +o pipefail

echo -e 'SUBSYSTEM=="drm", ACTION=="change", RUN+="/opt/ehs/bin/monitor_control.sh"' > /etc/udev/rules.d/40--external-display.rules

service udev restart
