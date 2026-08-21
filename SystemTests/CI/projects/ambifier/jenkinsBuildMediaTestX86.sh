#!/bin/bash

#get current directory so we don't have to worry about paths
#taken from https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
#go to the root of EHS
cd ${DIR}/../../../../
INX_SKIP_VAGRANT=true ./configure linux_x86_gtk_gst_ambifier2_vagrant

VM=""

pushd ./target/envbuildscripts/vm-build-environments/${VM}
vagrant up
#set up SSH
vagrant ssh -- -t 'cp /vagrant_data/media_scheduler_test/scripts/buid-deploy/vagrant-repo-user-private-key ~/.ssh/id_rsa && cp /vagrant_data/media_scheduler_test/scripts/buid-deploy/vagrant-repo-user-private-key.pub ~/.ssh/id_rsa.pub && chmod og-rwx /home/vagrant/.ssh/id_rsa && chmod og-rwx /home/vagrant/.ssh/id_rsa.pub'
#do make clean separately as it often fails
vagrant ssh -- -t 'cd /vagrant_data/media_scheduler_test && make clean'
vagrant ssh -- -t 'cd /vagrant_data/media_scheduler_test && make targetenv_cleanall && make prepdeps && make -j 4 && make targetenv'
vagrant ssh -- -t 'TIME_STAMP=`date +%F-%Hh%Mm%Ss` && cd /vagrant_data/media_scheduler_test && sed -i "1s/.*/$TIME_STAMP-ci/" ../TARGET_TREES/ehs_env-linux_x86_gtk_gst_ambifier2_vagrant/sysdata/version.nfo'
vagrant ssh -- -t 'cd /vagrant_data/media_scheduler_test && make PATCH_NAME=jenkins_x86_ambifier2_vagrant DEVMANSERVER=devman.inx-systems.com DEVMANSSHPORT=8822 DEVMANUID=root upload_ehs_sys_patch'
vagrant halt
popd



#* upload_ehs_sys_patch - Uploads TARGETENV tree package to the default devman server or to the specificed server using:
#*		              - Use VERSION_NAME=[your version name] to give the build a special name.
#*                      - e.g. make DEVMANSERVER=[your.url.com] DEVMANUID=[your username] upload_ehs_sys_patch.
#*                      - If the patch requires a server reboot (i.e. because it has a new start-upo script) then
#*                        set an additional variable SYSPATCH_NEED_REBOOT=yes on the command line.
#*                        (KEEP_USERCONFIG=yes & KEEP_APPLICATION=yes can also be used here as described above).



