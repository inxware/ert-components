#!/bin/bash
# P Drezet 2008
./target/envbuildscripts/targetenv_linux_x86_gtk.sh linux_x86_gtk linux_x86_gtk_minibox


echo Copying PicoLCD binaries to target tree at: ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
svn --force export ../OTHER_DEPENDENCIES/USBLCD/$SPECIFIC_TARGET/ ../TARGET_TREES/ehs_env-$SPECIFIC_TARGET/bin/
echo Done

#Put libraries for nohrtec box in bin folder
echo Copying extra libraries the IEI needs.
svn --force export ../OTHER_DEPENDENCIES/Libs/linux_x86_gtk_minibox/ ../TARGET_TREES/ehs_env-linux_x86_gtk_minibox/bin/
