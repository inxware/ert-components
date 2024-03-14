#!/bin/bash

# Use the host's gstreamer installation

echo APPLYING BLAZE BOARD HACKS

rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgst*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/gstreamer*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/csdir/gstreamer*



