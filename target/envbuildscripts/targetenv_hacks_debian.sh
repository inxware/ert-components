#!/bin/bash
TARGET=$1

### todo - is this file obsolete? does anything use it?

#make a debian install

./target/envbuildscripts/targetenv_remove_gtk_gst_libs.sh
#Add the home desktop app
#export NO_AUTOSTART=yes
#./target/envbuildscripts/targetenv_makeprod.sh $TARGET
./target/envbuildscripts/targetenv_make_deb.sh $TARGET



