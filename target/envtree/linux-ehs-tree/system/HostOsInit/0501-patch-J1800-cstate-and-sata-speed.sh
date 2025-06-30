#!/bin/bash
export PATH=$PATH:/usr/sbin
apt-get -y install intel-microcode
sed "s/^GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT=\"quiet libata.force=1.5 intel_idle.max_cstate=1\"/" /etc/default/grub > grub.tmp && mv grub.tmp /etc/default/grub

update-grub

