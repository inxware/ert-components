




echo "Removing library files to be provided by host"


rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/csdir/*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/corelib/*

rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/gst*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/alsa*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/cairo*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/engines*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/gdk*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/gio*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/glib*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/gtk*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/pango*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/pkgconfig*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/python*
rm -rf ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/X11*


rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libasound*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libatk*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgtk*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgst*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgst*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libav*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libcairo*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libfaad*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libfont*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libfreetype*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libcairo*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libglib*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgobject-2.0.so*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgio*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgdk*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgmod*
# needed afterall rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libpng*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libpango*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libpixman*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libgtk*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libjpeg*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libmad*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/librsvg*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libX*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libxcb*
rm -f ../TARGET_TREES/ehs_env-$TARGET/bin/cslib/libz*



# Some sound fix ups Is this needed?
if [ 0 == 1 ]; then
pushd .
cd ../TARGET_TREES/ehs_env-$TARGET/bin/cslib
ln -fs /usr/lib/libasound.so.2 libasound.so.2
ln -fs /usr/lib/libasound.so.2 libasound.so.2.0.0
# This should be done by the lib but we'll do it again here as we don't trust it
#ln -fs libfaad.so.2 libfaad.so.0
popd
fi

