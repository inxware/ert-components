#!/bin/bash
TARGET=$1

#obsolete???? We build the tools in a tools-specific installer now (at least for windows)
#make a debian install

./target/envbuildscripts/targetenv_remove_gtk_gst_libs.sh
#Add the home desktop app. We always want an inx config for EHS with tools, so set production
./target/envbuildscripts/targetenv_makeprod.sh $TARGET
#Package everything into a single deb
./target/envbuildscripts/targetenv_make_deb.sh $TARGET --with-tools




