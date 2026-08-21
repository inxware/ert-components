#!/bin/bash
# This script should be used on first boot only. It sets up the debian jessie deb repo when the 
# system first runs after a USB stick install
export PATH=$PATH:/usr/sbin
set +e
set +o pipefail

#create the Host installer directory for legacy EHSS devices if we are run from devman
mkdir -p $INXWAREROOT/bin/HostOsInit/

#Don't do any of this on a devboard
test -f /opt/devboard && exit
	

#On first boot nothing works properly,so reboot a normal boot for apt-get to work
if [ ! -f $INXWAREROOT/bin/HostOsInit/firstboot ]; then
	touch $INXWAREROOT/bin/HostOsInit/firstboot
	rm $INXWAREROOT/bin/HostOsInit/0000-doing	
	reboot
	exit
else 
	rm $INXWAREROOT/bin/HostOsInit/firstboot
fi

# deb http://repo.ambifier.com/mirror jessie main
# append the usual debian server
echo -e "
deb http://repo.ambifier.com/mirror/ jessie main

deb http://security.debian.org/ jessie/updates main
deb-src http://security.debian.org/ jessie/updates main

deb http://ftp.uk.debian.org/debian jessie main non-free



deb http://httpredir.debian.org/debian jessie-backports main contrib non-free
deb-src http://httpredir.debian.org/debian jessie-backports main contrib non-free

" > /etc/apt/sources.list || :

#Try to avoid contention with other apt  dpkg stuff (though solved by reboot after install	
#while fuser /var/lib/dpkg/lock &>/dev/null ; do
#    sleep 1
#done 

# This next line crashes out the script if run without a reboot despite any attempts to avoid contention
(
RETRYCOUNT = 0;
until  apt-get -y update &>/dev/null || test $RETRYCOUNT -ge 10   ; do
	sleep 1
	RETRYCOUNT=$(($RETRYCOUNT + 1))
done
)  || :


#fix a bug in simple-cdd installer, that does not install start-stop-daemon
touch /sbin/start-stop-daemon &>/dev/null || :
chmod +x /sbin/start-stop-daemon &>/dev/null || :
(
#while fuser /var/lib/dpkg/lock &>/dev/null ; do
#    sleep 1
#done 

# This MUST BE INSTALLED FOR Ambifier Constroller Daemon to work 
#until 
apt-get -y install dpkg &>/dev/null 
# do
#	sleep 1
# done
)  || :



