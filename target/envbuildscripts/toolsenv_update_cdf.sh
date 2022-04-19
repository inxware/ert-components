#!/bin/bash
homePath=`eval echo "~"`
winePath="${homePath}/.wine/drive_c/users/patrick/Application Data/inx/brix"
if ! [ -d "${winePath}" ]; then
	winePath="${homePath}/.wine/drive_c/users/pdrezet/Application Data/inx/brix"
fi
if [ -d "${winePath}" ]; then
    echo "copying to ${winePath}"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "${winePath}/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "${winePath}/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "${winePath}/BMP/"  \;
fi

newToolsPath="${homePath}/work/inx/tools/V2-OpenGL-Wx/brix-tools/iab/dist"
if [ -d "${newToolsPath}" ]; then
    echo "copying to ${newToolsPath}"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "${newToolsPath}/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "${newToolsPath}/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "${newToolsPath}/BMP/"  \;
fi

if [ -d "../dist/CDF" ]; then
    echo "copying to ../dist/"
    find ./Common/Components/ -iname '*.idf.*' -exec cp {} "../dist/IDF/"  \;
    find ./Common/Components/ -iname '*.cdf' -exec cp {} "../dist/CDF/"  \;
    find ./Common/Components/ -iname '*.bmp' -exec cp {} "../dist/BMP/"  \;
else
    echo "Please check out the dist path"
fi
