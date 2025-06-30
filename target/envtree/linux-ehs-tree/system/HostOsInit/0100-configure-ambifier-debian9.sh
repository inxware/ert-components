#!/bin/bash
set +e
set +o pipefail

#remove the DEVMAN media plugin URL if it has been set different to core.
rm -f /opt/ehs/devman/plugins/1/DEVMANPLAYERURL.000 &> /dev/null || :

mkdir -p /opt/ehs/userdata/media/ &> /dev/null || :
#cp patches/failover.mp3 /opt/ehs/userdata/media/

#setup automount USB
#USB loader
apt-get update
apt-get -y install pmount &> /dev/null ||:

echo -e "
[Unit]
Description=Mount USB sticks
BindsTo=dev-%i.device
After=dev-%i.device 

[Service]

Type=oneshot

RemainAfterExit=yes

ExecStart=/usr/local/bin/cpmount /dev/%I
ExecStop=/usr/bin/pumount /dev/%I
" >  /lib/systemd/system/usbstick-handler@.service

echo -e "
mountpoint -q /media/usb_media && umount /media/usb_media
test -d /media/usb_media || mkdir /media/usb_media
/usr/bin/pmount --umask 000 --noatime -w --sync $1 usb_media
" > /usr/local/bin/cpmount
chmod +x /usr/local/bin/cpmount

echo -e "
ACTION==\"add\", KERNEL==\"sd[a-z][0-9]\", TAG+=\"systemd\", ENV{SYSTEMD_WANTS}=\"usbstick-handler@%k\"
" > /etc/udev/rules.d/11-media-by-label-auto-mount.rules