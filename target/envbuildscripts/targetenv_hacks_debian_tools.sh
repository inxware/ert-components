#!/bin/bash
TARGET=$1


#make a debian install

./target/envbuildscripts/targetenv_remove_gtk_gst_libs.sh
#Add the home desktop app. We always want an inx config for EHS with tools, so set production
./target/envbuildscripts/targetenv_makeprod.sh $TARGET
#Make the .deb file

./target/envbuildscripts/targetenv_make_deb.sh $TARGET --with-tools




