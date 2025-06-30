#!/bin/bash
# inx limited 2012
# Creates a Debian .deb package and removes duplicate libaries from EHS tree that 
# can be installed as dependencies for debian

#TODO2023 all the ambifier specific bits of this need to changed the EHS_DEBIAN_VERSION and $GNU_ARCH instead!

set -e

echo "######################################### BUILDING DEBIAN PACKAGE ##############################################"
echo "****************************************************************************************************************"
echo "**  Making Debian package - ONLY RUN ME AS A make targetenv_* option - not standalone                         **"
echo "****************************************************************************************************************"

export SPECIFIC_TARGET=$1
if [ "$2" = "--with-tools" ]; then
	export NO_AUTOSTART=yes
	export WITH_TOOLS=yes
	echo "WARNING: Including inxware tools in the runtime packager OK?"
	read -n 1
fi

if [ "$2" = "--no-autostart" ]; then
	export NO_AUTOSTART=yes
	echo "Not setting autostart for this package"
else
	echo "Setting autostart for this package"
fi

echo "#################################################################################################################"
echo "### Run me after make targetenv                            "
echo "### Build parameters: "
echo "### EHS_GNU_OS               --> ${EHS_GNU_OS}"
echo "### EHS_GNU_ARCH             --> ${EHS_GNU_ARCH}"
echo "### EHS_OS                   --> ${EHS_OS}"
echo "### EHS_GNU_ARCH             --> ${EHS_ARCH}"
echo "### SPECIFIC_TARGET          --> ${SPECIFIC_TARGET}"
echo "### SYSTEM_VARIANT           --> ${SYSTEM_VARIANT}"
echo "### EHS_DEBIAN_VERSION       --> ${EHS_DEBIAN_VERSION}"
echo "### INXWARE_TARGETENV_HACKS  --> ${INXWARE_TARGETENV_HACKS}"
echo "#################################################################################################################"

EHS_VERSION_SUGGEST=`sed -n 1p ../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/sysdata/version.nfo`
echo "Suggested EHS version string is ${EHS_VERSION_SUGGEST}"
#@todo need tie this version in with a build number, repo number, release number etc.
if [ -z "${EHS_VERSION}" ]; then
	echo "No version set in  EHS_VERSION, using version.nfo file value (${EHS_VERSION_SUGGEST})."
	echo "use make EHS_VERSION=2.x.x targetenv_deb"
	#echo "ctrl-C to exit or continue with ${EHS_VERSION_SUGGEST}"
	#read -n 1
	EHS_VERSION="${EHS_VERSION_SUGGEST}"
	echo "Found EHS version = ${EHS_VERSION}"
fi

#Make a base directory that we will copy the runtime into  
DEBIAN_WORKING_BASE="../TARGET_TREES/ehs_deb-$SPECIFIC_TARGET/"
test -e "${DEBIAN_WORKING_BASE}/debian/opt/" && rm -Rf  "${DEBIAN_WORKING_BASE}" 
mkdir -p "${DEBIAN_WORKING_BASE}/debian/opt/" || echo "Could not create debian directory"
mkdir -p "${DEBIAN_WORKING_BASE}/debian/etc/init.d"  
mkdir -p "${DEBIAN_WORKING_BASE}/debian/DEBIAN"  

source target/envbuildscripts/targetenv_debian_utils.sh

if [ "${NO_AUTOSTART}" != "yes" ]; then
	WriteInitDFile
fi
echo "###############################################################"
#ls "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/"
echo "Creating a Debian staging directory using:"
echo "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/ -> ${DEBIAN_WORKING_BASE}/debian/opt/ehs/"
echo "Copying the targetenv tree to the Debian directory" 
cp -Rf "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/" "${DEBIAN_WORKING_BASE}/debian/opt/ehs/"
echo "###############################################################"
echo "... done copying files"
# Some belt and braces clean up if not done properly already 
touch ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/default/__not_cleaned.nfo
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/sysdata/ehslog.csv
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/ehs_tcpip.log
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/sysdata/*.flag
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/sysdata/var/*
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/devman/core/var/*
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/devman/core/*.flag
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/devman/plugins/0/*.flag
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/devman/plugins/1/*.flag
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/devman/plugins/2/*.flag
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/default/.gitignore
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/temp/.gitignore
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/temp/*
rmdir ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/temp/
rm -f ${DEBIAN_WORKING_BASE}/debian/opt/ehs/sysdata/app2run.nfo
touch ${DEBIAN_WORKING_BASE}/debian/opt/ehs/appdata/default/__cleaned.nfo

if [ -n "${WITH_TOOLS}" ]; then
	cp  "../dist/installers/setup-inxware-tools-without-runtime.exe" "${DEBIAN_WORKING_BASE}/debian/opt/ehs/"
fi

#Make the control file for the DEBIAN directory
if [ -n "${WITH_TOOLS}" ]; then
	echo "Package: inxware" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
else
#todo the next switch should select between commercial/free versions perhaps?
# old crap way of doing it:
#	if [ "${SYSTEM_VARIANT}" = "ambifier" -o "${SYSTEM_VARIANT}" = "ambifier2" -o "${SYSTEM_VARIANT}" = "ambifier2-adnoc" ]; then
#		echo "Package: ehs" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#		echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#	else 
#		if [ "${SYSTEM_VARIANT}" = "ambifier-debug" -o "${SYSTEM_VARIANT}" = "ambifier2-debug" -o "${SYSTEM_VARIANT}" = "ambifier2-deb11"  ]; then
#			echo "Package: ehs-debug"                 > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#			echo "Version: 0:${EHS_VERSION}-debug-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#		else
#			echo "NOt found any of the specific targets so building the inxware package"
#			echo "Package: inxware-ert"                > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#			echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#		fi
#	fi
	if [ "${DEBIAN_PACKAGE_NAME}" = "" ]; then
		echo "Package: ehs" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
		echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	else 
			echo "platform config.mk specifies devian package name as ${DEBIAN_PACKAGE_NAME}"
			echo "Package: ${DEBIAN_PACKAGE_NAME}" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
			echo "Version: 0:${EHS_VERSION}-1"     >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	fi
fi

echo "Installed-Size: 5000"      >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#echo "..license ... updates .." >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Section: middleware"       >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Priority: standard"        >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
# Note we may have over used SYSTEM_VARIANT for some of therse options? Perhaps we need an additional level for DEPLOYMNENT_VARIANT or something

#TODO2024 - All these packages can be defgined in os-arch target.mk and platform config.mk files really
#echo "EHS_GUI_SUPPORT=${EHS_GUI_SUPPORT}"
if [ "${EHS_GUI_SUPPORT}" = "lvgl" ]; then
    if [ "${DEBIAN_PACKAGE_PLATFORM_EXTRA}" != "" ];then
		DEBIAN_PACKAGE_PLATFORM_EXTRA="${DEBIAN_PACKAGE_PLATFORM_EXTRA},"
	fi
	export DEBIAN_PACKAGE_EXTRA="${DEBIAN_PACKAGE_PLATFORM_EXTRA}libsdl2-2.0-0,libarchive13,libgl1-mesa-dri"
else
	export DEBIAN_PACKAGE_EXTRA=xorg
fi

# insert other debian package dependencies
# opencv
if [ "${EHS_MV_SUPPORT}" = "opencv" ]; then
	echo "******** Including opencv dependency ********"
	export DEBIAN_PACKAGE_EXTRA="${DEBIAN_PACKAGE_EXTRA},libopencv-dev"
	# or use non -dev
	# runtime opencv 4.6.0, seems to be default on on rpi debian12
	# export DEBIAN_PACKAGE_EXTRA="${DEBIAN_PACKAGE_EXTRA},libopencv-core406,libopencv-imgproc406,libopencv-highgui406"
fi
# libcamera
if [ "${EHS_USE_LIBCAMERA}" != "" ]; then
	echo "******** Including libcamera dependency ********"
	export DEBIAN_PACKAGE_EXTRA="${DEBIAN_PACKAGE_EXTRA},libcamera-apps"
fi

# TODO2023 - this should be changed to a specific set of dependencies defined in config.mk
echo "Building package for SYSTEM_VARIANT=$SYSTEM_VARIANT"

if [ "${SYSTEM_VARIANT}" = "ambifier2" -o "${SYSTEM_VARIANT}" = "ambifier-deb11" -o "${SYSTEM_VARIANT}" = "ambifier2-debug" \
	-o "${SYSTEM_VARIANT}" = "ambifier2-adnoc" -o "${SYSTEM_VARIANT}" = "ambifier2-deb11" ]; then
#### AMBIFIER BUILDS ### 
  #echo "Depends: xorg,ambifier" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  #todo these packages should mostly if not all be in DEBIAN_PACKAGE_EXTRA
  if [ "${EHS_DEBIAN_VERSION}" = "11" ]; then
  	echo "Depends: ${DEBIAN_PACKAGE_EXTRA},ambifier,lm-sensors,libarchive13,libxml2,libpng16-16,libgstreamer1.0-0,libgstreamer-plugins-base1.0-0,gstreamer1.0-plugins-good,gstreamer1.0-plugins-bad,gstreamer1.0-plugins-ugly,libgtk2.0-0,libcurl4" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  else	
	echo "Depends: ${DEBIAN_PACKAGE_EXTRA},ambifier,lm-sensors,libarchive13,libxml2,libpng16-16,libgstreamer1.0-0,libgstreamer-plugins-base1.0-0,gstreamer1.0-plugins-good,gstreamer1.0-plugins-bad,gstreamer1.0-plugins-ugly,libgtk2.0-0,libcurl3" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  fi
  ##todo2022 these two arch options should be made as one outside of the SYSTEM VARIANT condition.
  if [ "${EHS_ARCH}" = "arm64" ];then
      echo "Architecture: arm64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  elif [ "${EHS_ARCH}" = "arm" ];then
      echo "Architecture: armhf" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  else
      echo "Architecture: amd64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  fi
else
#### GENERAL BUILDS ###
 if [ "${EHS_GNU_ARCH}" = "arm64" ];then
      echo "Architecture: arm64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  elif [ "${EHS_GNU_ARCH}" = "arm" ];then
      echo "Architecture: armhf" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  elif [ "${EHS_GNU_ARCH}" = "amd64" ];then
      echo "Architecture: amd64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  elif [ "${EHS_GNU_ARCH}" = "x86_64" ];then
      echo "Architecture: amd64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"	
  else
      echo "Architecture: i386" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
   fi
#echo "Depends: " >> ./debian/DEBIAN/control
#todo2023 - see above this should be replaced by config.mk
  if [ "${SYSTEM_VARIANT}" = "ambifier" -o "${SYSTEM_VARIANT}" = "ambifier-debug" ]; then
    echo "Depends: ${DEBIAN_PACKAGE_EXTRA},ambifier" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  else
    if [ -n "${WITH_TOOLS}" ]; then
      echo "Depends: ${DEBIAN_PACKAGE_EXTRA},wine" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	elif [ "${SYSTEM_VARIANT}" = "msg200_supervisor" ]; then
	  echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      echo "Not adding any dependencies (\$SYSTEM VARIANT=supervisor)" 
	  echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    else
	  echo "Depends: ${DEBIAN_PACKAGE_EXTRA}" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
    fi
  fi
fi

# Allow plain "ehs.deb" versions to be replaced with others.

# If we are default ehs then allow replacement of ehs-debug
if [ "${DEBIAN_PACKAGE_NAME}" = "" -o "${DEBIAN_PACKAGE_NAME}" = "ehs" ] ; then
	echo 'Replaces: ehs-debug' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	#echo 'Breaks: ehs' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control" 
else
	echo 'Replaces: ehs' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	#echo 'Breaks: ehs-debug' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control" 
fi

echo "Bugs: support.inx-systems.net" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Maintainer: inx limited UK, <info@inx-systems.com>" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Homepage: http://www.inx-systems.com" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
if [ -n "${WITH_TOOLS}" ]; then
echo "Description: BRIX Tools & Runtime" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
else
echo "Description: Runtime environment for inxware applications" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
fi
echo " Enables networked embedded applications to be easily generated, deployed and maintained. To build applications download the free tools available www.inx-systems.com" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"



#Setup the pre-processing script - Stop EHS first
echo "#!/bin/bash" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
if [ "${NO_AUTOSTART}" != "yes" ]; then
	echo "test -e /etc/init.d/ehs_run && /etc/init.d/ehs_run stop" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
fi
echo "rm -Rf /opt/ehs/bin" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
echo "rm -Rf /opt/ehs/appdata/*" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
echo "rm -Rf /opt/ehs/devman/*" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
echo "rm -Rf /opt/ehs/sysinfo/*" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
#echo "rm -Rf /opt/ehs/userdata" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
#echo "test -e /etc/init.d/ehs_run && /etc/init.d/ehs_run start" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"

#Setup the post processing script
#Make auto start
echo "#!/bin/bash" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo "chmod -R a+rw /opt/ehs"  >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"

if [ -n "${WITH_TOOLS}" ]; then

	## The Ubuntu installer isn't running as sudo - so need to find the username two ways!!!
	echo 'if [[ $USER == "root" ]]; then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '	INX_USER=$SUDO_USER' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo 'else' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '	INX_USER=$USER' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo 'fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#if we are running as the Ubuntu installer will need to install as root
	echo 'if [ -z "$INX_USER" ] ;then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#assume we are already root - need a hack to find the likely user name if we are in a bare root shell
	#echo ' users /var/log/wtmp' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' USERSLOGGEDIN=(`users /var/log/wtmp`)' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' echo USERSLOGGEDIN= ${USERSLOGGEDIN[*]} ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' echo USERSLOGGEDIN-1=${USERSLOGGEDIN[2]}' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' for U in ${USERSLOGGEDIN[@]}; do' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '   if [ \"$U\" != \"root\" ]; then ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '     GUESSEDUSER=$U ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '   break ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '   fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' done' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' export USER=$GUESSEDUSER' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo ' echo GUESSED USER= $GUESSEDUSER ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '	 sudo -u $GUESSEDUSER DISPLAY=:0.0 WINEPREFIX=/home/$GUESSEDUSER/.wine wine /opt/ehs/setup-inxware-tools-without-runtime.exe /SILENT ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '	 chown -R $GUESSEDUSER:$GUESSEDUSER /home/$GUESSEDUSER/.wine/cdrive/users/$GUESSEDUSER/Application\ Data/inx' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	# if you need to see th logs exit with -1: echo ' exit 1' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"

	# 2> /dev/null || echo Could not install the tools' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo 'else' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo '	 sudo -u $INX_USER wine /opt/ehs/setup-inxware-tools-without-runtime.exe /SILENT 2> /dev/null || echo Could not install the tools' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo 'fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"


	#echo 'env' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo 'echo installing tools for USER=\"$INX_USER\" USER_ORIG=\"$USER\" SUDO_USER=\"$SUDO_USER\"' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"

	# Add the menu desktop files to the RFS
	# iAB:
	#create a shared tools run script - .desktop doesn't like launchers with user's env variables in it...
	#create these in the opt area then copy them to the sys so they don't get uninstalled when EHS is re-installed

	mkdir -p "${DEBIAN_WORKING_BASE}/debian/usr/bin"
	echo "#!/bin/sh" > "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/iab"
	#@todo we install in brix - hard coded!!!
	echo "WINEPREFIX=\"/home/\$USER/.wine\" wine \"/home/\$USER/.wine/drive_c/users/\$USER/Application Data/inx/brix/bin/inxware Application Builder.exe\"" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/iab"
	chmod +x  "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/iab"

	echo "#!/bin/sh" > "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/igb"
	echo "WINEPREFIX=\"/home/\$USER/.wine\" wine \"/home/\$USER/.wine/drive_c/users/\$USER/Application Data/inx/brix/bin/inxware GUI Builder.exe\"" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/igb"
	chmod +x  "${DEBIAN_WORKING_BASE}/debian/opt/ehs/bin/igb"

	#copy in the icons too
	####mkdir -p "${DEBIAN_WORKING_BASE}/debian/opt/ehs/share/icons"
	cp ../dist/*icon.png "${DEBIAN_WORKING_BASE}/debian/opt/ehs/"

	#create the launcher .desktop file
	#####mkdir -p "${DEBIAN_WORKING_BASE}/debian/usr/share/applications" 
	## @todo we should copy the files from ./targetenv/system/ here!

	echo "[Desktop Entry]" > "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Name=BRIX Application Builder" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Categories=Development;Application;" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Comment=BRIX Application Development Tool" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Exec=/usr/bin/iab" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Type=Application" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "StartupNotify=true" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	echo "Icon=/usr/share/icons/iab-icon.png" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop" # found in /usr/share/icons
	echo "" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"
	chmod +x "${DEBIAN_WORKING_BASE}/debian/opt/ehs/iab.desktop"

	# iGB:
	## @todo we should copy the files from ./system here!
	echo "[Desktop Entry]" > "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	echo "Name=BRIX GUI Builder" >> "${DEBIAN_WORKING_BASE}/debian///opt/ehs/igb.desktop"
	echo "Categories=Development;Application;" >> "${DEBIAN_WORKING_BASE}/debian///opt/ehs/igb.desktop"
	echo "Comment=BRIX GUI Layout Tool" >> "${DEBIAN_WORKING_BASE}/debian///opt/ehs/igb.desktop"
	echo  "Exec=/usr/bin/igb" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	echo "Type=Application" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	echo "StartupNotify=true" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	echo "Icon=/usr/share/icons/igb-icon.png"  >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	echo "" >> "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"
	chmod +x "${DEBIAN_WORKING_BASE}/debian/opt/ehs/igb.desktop"

	echo 'echo Installed inxware tools successfully - installed icons in menu' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"



fi #end installed tools 

# EHS:
# Don't need to copy the links for this as this is common to tools and no tools versions and will be updated for each install
mkdir -p "${DEBIAN_WORKING_BASE}/debian/usr/share/applications" 
## @todo we should copy the files from ./system here!
echo "[Desktop Entry]" > "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Name=eRT" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Categories=Development;Application;" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Comment=The Inxware Lucid Runtime" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Exec=/opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Type=Application" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "StartupNotify=true" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Icon=/opt/ehs/inx-icon.png" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
chmod +x "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"


#Add bits to the postinst to copy the launcher files to the system bits
echo 'mkdir -p /usr/share/applications' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'mkdir -p /usr/share/icons' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'cp -f /opt/ehs/*.desktop /usr/share/applications' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'cp -f /opt/ehs/*-icon.png /usr/share/icons' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"

if [ -n "${WITH_TOOLS}" ]; then
	echo 'cp /opt/ehs/bin/iab /usr/bin/' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	echo 'cp /opt/ehs/bin/igb /usr/bin/' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
fi

if [ -n "${NO_AUTOSTART}" ]; then
	#start EHS as user in the background
	pidof -x run_ehs.sh && killall run_ehs.sh
	pidof ehs.exe && killall ehs.exe
	# disable start-up after install to avoid running it as root, and creating non-acessible temp dir
	# @TODO - this may be fixed with using $SUDO_USER instead, provided that we want to launch it after installation
	#start it up without a daemon
	#echo 'if [ -n "${USER}" ]; then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo 'sudo -u $USER -b nohup /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo 'else' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#if we didn't instal tools we need to guess the user for the ubuntu installer
	#echo '  if [ -z "${GUESSEDUSER}" ]; then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '    USERSLOGGEDIN=(`users /var/log/wtmp`)' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '    echo USERSLOGGEDIN= ${USERSLOGGEDIN[*]} ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '    echo USERSLOGGEDIN-1=${USERSLOGGEDIN[2]}' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '    for U in ${USERSLOGGEDIN[@]}; do' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '       if [ \"$U\" != \"root\" ]; then ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '          GUESSEDUSER=$U ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '          break ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '       fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '    done' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '   fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo '   sudo -u $GUESSEDUSER -b  nohup /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	#echo 'fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
else
	#start as root 
	if [ "${NO_AUTOSTART}" != "yes" ]; then
		echo "update-rc.d -f run_ehs remove" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
		echo "update-rc.d run_ehs defaults 50 30" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
		echo "sleep 1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
		echo "test -e /etc/init.d/run_ehs && /etc/init.d/run_ehs restart" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
	fi
fi
#Make the debian installer scripts executable
chmod 0755 "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
chmod 0755 "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"

#And create the package
find "${DEBIAN_WORKING_BASE}/debian" -type d | xargs chmod 755
pushd "${DEBIAN_WORKING_BASE}"
fakeroot dpkg-deb --build debian ./
popd

echo "*****************************************************************************************************************"
echo " Debian package built:"
ls -l ../TARGET_TREES/ehs_deb-${SPECIFIC_TARGET}/*.deb
echo " Use 'make upload_ehs_deb UPLOAD=<ssh url to Devman enabled debian server>' to upload for deployment"
echo " or"
echo " Use 'make targetenv_upload_appland' to upload to the Appland (only if supported)"
echo "*****************************************************************************************************************"

echo "######################################### FINISHED BUILDING DEBIAN PACKAGE ######################################"
