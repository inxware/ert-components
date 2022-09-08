#!/bin/bash
# P Drezet 2008
./target/envbuildscripts/targetenv_linux_x86_gtk.sh linux_x86_gtk linux_x86_gtk_iei

#Put libraries for nohrtec box in bin folder
echo Copying extra libraries the Nohrtec needs.
svn --force export ../OTHER_DEPENDENCIES/Libs/linux_x86_gtk_iei/ ../TARGET_TREES/ehs_env-linux_x86_gtk_iei/bin/