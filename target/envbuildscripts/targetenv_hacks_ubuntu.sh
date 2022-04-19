#!/bin/bash


echo Gstreamer alsa audio libraries hack
# Avoid compaints from the new alsa build with older libraries?
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libasound*
pushd .
cd ../TARGET_TREES/ehs_env-$TARGET/bin/cslib
ln -fs /usr/lib/libasound.so.2 libasound.so.2
ln -fs /usr/lib/libasound.so.2 libasound.so.2.0.0

# This should be done by the lib but we'll do it again here as we don't trust it
ln -fs libfaad.so.2 libfaad.so.0

popd

echo Removing X and G libraries....
./target/envbuildscripts/targetenv_remove_gtk_gst_libs.sh


