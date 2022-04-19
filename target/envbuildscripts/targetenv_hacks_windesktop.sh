#!/bin/bash

echo "To avoid problems with different version of iconv.dll in system we need put this in PWD as path DLLs are last in the line"
# Avoid compaints from the new alsa build with older libraries?
pushd .
cd ../TARGET_TREES/ehs_env-$TARGET/bin/
mv cslib/*.dll ./
#Add the home app
popd

./target/envbuildscripts/targetenv_makeprod.sh $TARGET


