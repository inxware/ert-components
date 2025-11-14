
if [ "${DEBIAN_WORKING_BASE}" = "" ]; then 
    echo "WARNING build variable DEBIAN_WORKING_BASE is not set! Exiting"
    exit 1
fi

function WriteInitDFile {

    #Make a start-stop script
    echo "#!/bin/bash" > "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
    echo "### BEGIN INIT INFO" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
    echo "# Provides:  ehs" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"

    # TODO This should just be a wait for network flag in the config.mk file
    echo "# Required-Start: $network $local_fs" >> "${DEBIAN_WORKING_BASE}/debian/etc/init.d/run_ehs"
    
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

}