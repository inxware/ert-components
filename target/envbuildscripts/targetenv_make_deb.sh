#!/bin/bash
# inx limited 2012
# Creates a Debian .deb package and removes duplicate libaries from EHS tree that 
# can be installed as dependencies for debian


echo "**************************************************************************************"
echo "**  Making Debian package - ONLY RUN ME AS A make targetenv_* option - not standalone **"
echo "**************************************************************************************"

if [  1 == 1 ]; then

export SPECIFIC_TARGET=$1
if [ "$2" == "--with-tools" ]; then
export NO_AUTOSTART=yes
export WITH_TOOLS=yes
#echo including tools OK?
#read -n 1
fi

if [ "$2" == "--no-autostart" ]; then
export NO_AUTOSTART=yes
fi


echo "########################################################"
echo " Run me after make targetenv                            "
echo "  Build parameters: "
echo "--> $EHS_GNU_OS"
echo "--> $EHS_GNU_ARCH"
echo "--> $EHS_OS"
echo "--> $EHS_ARCH"
echo "########################################################"

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

#make a copy of our cosey runtime 
pwd
ls "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/"
#echo "XXXX1"
cp -Rf "../TARGET_TREES/ehs_env-${SPECIFIC_TARGET}/" "${DEBIAN_WORKING_BASE}/debian/opt/ehs/"

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
#touch "${DEBIAN_WORKING_BASE}/debian/DEBIAN/conffiles"
#echo '/etc/ehs.conf' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/conffiles"
#echo '/etc/init.d/ehs' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/conffiles"

#Make the control file for the DEBIAN directory
if [ -n "${WITH_TOOLS}" ]; then
	echo "Package: brix" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
else
#todo the next switch should select between commercial/free versions perhaps?
	if [ "${SYSTEM_VARIANT}" == "ambifier" -o "${SYSTEM_VARIANT}" == "ambifier2" -o "${SYSTEM_VARIANT}" == "ambifier2-deb11" -o "${SYSTEM_VARIANT}" == "ambifier2-adnoc" ]; then
		echo "Package: ehs" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
		echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
	else 
		if [ "${SYSTEM_VARIANT}" == "ambifier-debug" -o "${SYSTEM_VARIANT}" == "ambifier2-debug" -o "${SYSTEM_VARIANT}" == "ambifier2-deb11"  ]; then
			echo "Package: ehs-debug"                 > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
			echo "Version: 0:${EHS_VERSION}-debug-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
		else
			echo "Package: brix"                > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
			echo "Version: 0:${EHS_VERSION}-1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
		fi
	fi
fi

echo "Installed-Size: 5000"      >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#echo "..license ... updates .." >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Section: middleware"       >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Priority: standard"        >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
# Note we may have over used SYSTEM_VARIANT for some of therse options? Perhaps we need an additional level for DEPLOYMNENT_VARIANT or something
if [ "${SYSTEM_VARIANT}" == "ambifier2" -o "${SYSTEM_VARIANT}" == "ambifier-deb11" -o "${SYSTEM_VARIANT}" == "ambifier2-debug" \
	-o "${SYSTEM_VARIANT}" == "ambifier2-adnoc" -o "${SYSTEM_VARIANT}" == "ambifier2-deb11" ]; then
  #echo "Depends: xorg,ambifier" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  if [ "${SYSTEM_VARIANT}" == "ambifier2-deb11" ]; then
  	echo "Depends: xorg,ambifier,lm-sensors,libarchive13,libxml2,libpng16-16,libgstreamer1.0-0,libgstreamer-plugins-base1.0-0,gstreamer1.0-plugins-good,gstreamer1.0-plugins-bad,gstreamer1.0-plugins-ugly,libgtk2.0-0,libcurl4" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  else	
	echo "Depends: xorg,ambifier,lm-sensors,libarchive13,libxml2,libpng16-16,libgstreamer1.0-0,libgstreamer-plugins-base1.0-0,gstreamer1.0-plugins-good,gstreamer1.0-plugins-bad,gstreamer1.0-plugins-ugly,libgtk2.0-0,libcurl3" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  fi
  echo "Architecture: amd64" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
else
echo "SYSTEM_VARIANT=$SYSTEM_VARIANT"
  echo "Architecture: i386" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#echo "Depends: " >> ./debian/DEBIAN/control
  if [ "${SYSTEM_VARIANT}" == "ambifier" -o "${SYSTEM_VARIANT}" == "ambifier-debug" ]; then
    echo "Depends: xorg,ambifier" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
  else
    if [ -n "${WITH_TOOLS}" ]; then
      echo "Depends: xorg,wine" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
    else 
      echo "Depends: xorg" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
    fi
  fi
fi
if [ "${SYSTEM_VARIANT}" == "ambifier-debug" -o "${SYSTEM_VARIANT}" == "ambifier2-debug" \
  -o "${SYSTEM_VARIANT}" == "ambifier2-adnoc" ] ; then
echo 'Replaces: ehs' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#echo 'Breaks: ehs' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control" 
else
echo 'Replaces: ehs-debug' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#echo 'Breaks: ehs-debug' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control" 
fi

# also  need - but breaks gtk2.0,gstreamer0.10
echo "Bugs: support.inx-systems.net" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Maintainer: inx limited UK, <info@inx-systems.com>" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
echo "Homepage: http://www.inx-systems.com" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
if [ -n "${WITH_TOOLS}" ]; then
echo "Description: BRIX Tools & Runtime" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
else
echo "Description: Runtime environment for inxware applications" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
fi
echo " Enables networked embedded applications to be easily generated, deployed and maintained. To build applications download the free tools available www.inx-systems.com" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/control"
#Make a start-stop script
echo "#!/bin/bash" > "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "### BEGIN INIT INFO" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "# Provides:  ehs" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
# TODO Ambifier2 - stope depending on csdctl
if [  "${SYSTEM_VARIANT}" == "ambifier" -o "${SYSTEM_VARIANT}" == "ambifier-debug" \
   -o "${SYSTEM_VARIANT}" == "ambifier2" -o "${SYSTEM_VARIANT}" == "ambifier2-debug" \
   -o "${SYSTEM_VARIANT}" == "ambifier2-deb11" -o "${SYSTEM_VARIANT}" == "ambifier2-adnoc" ]; then
echo '# Required-Start: $network $local_fs' >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo '# Should-Start: csdctl' >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs" 
else
echo "# Required-Start: $network $local_fs" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
fi
echo "# Required-Stop:" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "# Default-Start:  2" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "# Default-Stop:  0 1 6" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "# Short-Description: inxware application runtime environment" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "# Description:  Start the ehs runtime" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "### END INIT INFO" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo ". /lib/lsb/init-functions" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "test -x /opt/ehs/bin/run_ehs.sh || exit 0" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "start() {" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    echo -n \"Starting EHS:\"" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
if [ -n "${NO_AUTOSTART}" ]; then
echo "   pidof -x run_ehs.sh || /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST DAEMONIZE" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
else
echo "   pidof -x run_ehs.sh || /opt/ehs/bin/run_ehs.sh YES_RESTART LIB_HOST DAEMONIZE" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
fi

#we might put a test here to runcp with LIBHOST if it fails
echo "}" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "stop() {" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    /opt/ehs/bin/stop_ehs.sh" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "}" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "restart() {" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    echo -n \"Restarting $binary: \"" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    stop" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    sleep 2" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    start" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "}" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "force-reload() {" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    echo -n \"Restarting $binary: \"" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    stop" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    sleep 2" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    start" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "}" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "case \"\$1\" in" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    start)" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    	start" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    ;;" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    stop)" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    	stop" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    ;;" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    status)" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    	echo \"not sure\"" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    ;;" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    restart|force-reload)" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    	restart" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    ;;" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    *)" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    	echo \"Usage: $0 {start|stop|status|restart|force-reload}\"" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "    ;;" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "esac" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
echo "" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
chmod 0755 "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"

#Setup the pre-processing script - Stop EHS first
echo "#!/bin/bash" > "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
echo "test -e /etc/init.d/ehs_run && /etc/init.d/ehs_run stop" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"
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
echo "Name=EHS" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Categories=Development;Application;" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
echo "Comment=The BRIX Runtime" >> "${DEBIAN_WORKING_BASE}/debian/usr/share/applications/ehs.desktop"
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
#start it up without a daemon
echo 'if [ -n "${USER}" ]; then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'sudo -u $USER -b nohup /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'else' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
#if we didn't instal tools we need to guess the user for the ubuntu installer
echo '  if [ -z "${GUESSEDUSER}" ]; then' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '    USERSLOGGEDIN=(`users /var/log/wtmp`)' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '    echo USERSLOGGEDIN= ${USERSLOGGEDIN[*]} ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '    echo USERSLOGGEDIN-1=${USERSLOGGEDIN[2]}' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '    for U in ${USERSLOGGEDIN[@]}; do' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '       if [ \"$U\" != \"root\" ]; then ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '          GUESSEDUSER=$U ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '          break ' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '       fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '    done' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '   fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo '   sudo -u $GUESSEDUSER -b  nohup /opt/ehs/bin/run_ehs.sh NO_RESTART LIB_HOST' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo 'fi' >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
else
#start as root 
echo "update-rc.d -f run_ehs remove" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo "update-rc.d run_ehs defaults 50 30" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo "sleep 1" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
echo "test -e /etc/init.d/run_ehs && /etc/init.d/run_ehs restart" >> "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
fi
#Make the debian installer scripts executable
chmod 0755 "${DEBIAN_WORKING_BASE}/debian/DEBIAN/postinst"
chmod 0755 "${DEBIAN_WORKING_BASE}/debian/DEBIAN/preinst"

#And create the package
find "${DEBIAN_WORKING_BASE}/debian" -type d | xargs chmod 755
pushd "${DEBIAN_WORKING_BASE}"
fakeroot dpkg-deb --build debian ./
popd

# Check to see if this needs to be installed in a Debian package server
test -z "${UPLOADPORT}" && export  UPLOADPORT=8822

fi

if [ -n "${UPLOAD}" ];then
	echo "UPLOADING deb package (SYSTEM_VARIANT=${SYSTEM_VARIANT} to ${UPLOAD} ..."
	UPLOAD_DOMAIN=` echo "${UPLOAD}" | cut -d':' -f1 `
	UPLOAD_PATH=` echo "${UPLOAD}" | cut -d':' -f1 --complement`
	scp -P ${UPLOADPORT} ../TARGET_TREES/ehs_deb-${SPECIFIC_TARGET}/*.deb "${UPLOAD}/"
if [ "${SYSTEM_VARIANT}" = "ambifier2" -o "${SYSTEM_VARIANT}" == "ambifier2-debug" ]; then
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./install64_command.sh || echo \"Done - Check messages!\""
elif [ "${SYSTEM_VARIANT}" = "ambifier2-deb11" ]; then
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./installdeb11_command.sh || echo \"Done - Check messages!\""
else
	echo "VARIANT=${SYSTEM_VARIANT}"
	ssh -p  ${UPLOADPORT} "${UPLOAD_DOMAIN}" "cd /"${UPLOAD_PATH}"/ && ./install_command.sh || echo \"Done - Check messages!\""
fi
	
else
	echo "Not UPLOADING deb package to any Debian package servers"
fi







