#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# Removes ehs process (maybe run within ld loader). 
# EHS will restart if it is run within a reload script.
########################################################################

killall ehs.exe
killall ld-linux.so.2




