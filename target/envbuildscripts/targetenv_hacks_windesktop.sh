#!/bin/bash

echo "To avoid problems with different version of iconv.dll in system we need put this in PWD as path DLLs are last in the line"
# Avoid compaints from the new alsa build with older libraries?
pushd .
cd ../TARGET_TREES/ehs_env-$TARGET/bin/
if [ -d "cslib/" ]; then
    mv cslib/*.dll ./
fi

#Add the home app
popd
./target/envbuildscripts/targetenv_makeprod.sh $TARGET

#  The following is an example of the DLLs that are normally required for a windows gtk build.
#  'todo 2022 this doesn't include gstreamer pluging DLLs, which may also be required to be copied into the cslibb directory

#if [[ "$SPECIFIC_TARGET" ==  "win_x86_gtk_gst" ]]; then
#    echo "Copy DLLs for Windows x86 GTK build."
#    EHS_BIN_SOURCE_DIR=../ert-contrib-middleware/target_libs/i686-mingw32posix_gtk-gst/build/bin
#    EHS_BIN_DEST_DIR=../TARGET_TREES/ehs_env-$TARGET/bin/
#    for f in \
#    libarchive-13.dll \
#    libatk-1.0-0.dll \
#    libb2-1.dll \
#    libbrotlicommon.dll \
#    libbrotlidec.dll \
#    libbz2-1.dll \
#    libcairo-2.dll \
#    libcairo-gobject-2.dll \
#    libcrypto-1_1.dll \
#    libcurl-4.dll \
#    libdatrie-1.dll \
##    libdatrie-1.dll \
#    libepoxy-0.dll \
#    libexpat-1.dll \
#    libffi-8.dll \
#    libfontconfig-1.dll \
#    libfreetype-6.dll \
#    libfribidi-0.dll \
#    libgcc_s_dw2-1.dll \
#    libgdk_pixbuf-2.0-0.dll \
#    libgdk-3-0.dll \
#    libgio-2.0-0.dll \
#    libglib-2.0-0.dll \
#    libgmodule-2.0-0.dll \
#    libgobject-2.0-0.dll \
#    libgraphite2.dll \
#    libgraphite2.dll \
#    libgstreamer-1.0-0.dll \
#    libgthread-2.0-0.dll \
#    libgtk-3-0.dll \
#    libharfbuzz-0.dll \
#    libiconv-2.dll \
#    libidn2-0.dll \
#    libintl-8.dll \
#    liblz4.dll \
#    liblzma-5.dll \
#    libnghttp2-14.dll \
#    libpango-1.0-0.dll \
#    libpangocairo-1.0-0.dll \
#    libpangoft2-1.0-0.dll \
#    libpangowin32-1.0-0.dll \
#    libpcre2-8-0.dll \
#    libpixman-1-0.dll \
#    libpng16-16.dll \
#    libpsl-5.dll \
#    libssh2-1.dll \
#    libssl-1_1.dll \
#    libstdc++-6.dll \
#    libthai-0.dll \
#    libunistring-2.dll \
#    libwinpthread-1.dll \
#    libxml2-2.dll \
#    libzstd.dll \
#    zlib1.dll \
#    ; do
#      if [ ! -f "$EHS_BIN_SOURCE_DIR/$f" ]; then
#        echo "Failed to find required file: "$EHS_BIN_SOURCE_DIR/$f""
#        exit 1
#      fi
#      cp -f "$EHS_BIN_SOURCE_DIR/$f" "$EHS_BIN_DEST_DIR"
#    done
#fi



