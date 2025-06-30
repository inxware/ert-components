#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# This will reboot the entire box. Some OSs need the X-safe reboot 
# to be used so this is tried first
########################################################################

#in case we are from a cron
export PATH=$PATH:/bin:/sbin:/usr/bin

if [ -e /usr/X11R6/bin/wmreboot ];then
	/usr/X11R6/bin/wmreboot
else
test -e	/sbin/reboot && /sbin/reboot || reboot
fi





