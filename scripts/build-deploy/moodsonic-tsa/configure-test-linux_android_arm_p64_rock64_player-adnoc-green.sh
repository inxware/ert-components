#!/bin/bash
echo "This script will set some temporary network configuration for testing the unit"
echo "... and then put it back when you are finished"
if [ "$1" = "" ]; then
	echo "use configure-test-linux_android_arm_p64_rock64_player-adnoc-green.sh <IP address of device>"
fi
echo "Press any key to start"
read -n 1
adb connect $1:5555
adb root
adb remount
# We may want to set the DNS using getprops for our local DNS however this doesn't seem to
# have an affect - at least when DHCP suggestests our local DNS server. Otherwise we may want to do this:
#adb shell "setprop net.dns1 10.241.185.1"
adb shell "echo 192.164.4.241 adnoc.inx-systems.net >> /etc/hosts"
#adb shell "setprop net.dns1 10.16.128.21"
#adb shell "setprop net.dns2 10.16.128.222"
#adb shell "settings put global ntp_server 10.216.128.21"
adb shell "sync"
sleep 10
echo "rebooting..."
adb shell "reboot"
