#!/bin/bash
set +e
set +o pipefail

#(
#apt-get -y install libgstreamer0.10-0 gstreamer0.10-plugins-base gstreamer0.10-plugins-ugly &>/dev/null ||:
#apt-get -y install gstreamer0.10-plugins-good &> /dev/null ||:
#apt-get -y install gstreamer0.10-alsa &> /dev/null ||:
#) || :
rm -f /opt/ehs/devman/plugins/1/DEVMANPLAYERURL.000 &> /dev/null || :
#echo "10.64.4.20" > /opt/ehs/devman/core/config/DEVMANURL.000

mkdir -p /opt/ehs/userdata/media/ &> /dev/null || :
#cp patches/failover.mp3 /opt/ehs/userdata/media/

#USB loader
apt-get update
#apt-get -y install pmount &> /dev/null ||:
echo -e "
Section \"ServerFlags\"
    Option \"DontVTSwitch\" \"false\"
EndSection
" > /etc/X11/xorg.conf ||:

#echo -e "HelloAmbifier101\nHelloAmbifier101\n" | passwd root &> /dev/null || :
#todo fully disable root ssh?
