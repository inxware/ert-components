#!/system/bin/sh


LOG_TYPE=$1
LOG_PATH=$2
LOG_ADDERSS=$3
LOG_DEVICEID=$4

TARGET_FILE=${LOG_PATH}/platformstatus.log

function PrintSysLogs(){
	echo "******************* Supervisor Version *******************"
	cat /etc/ehs_supervisor/version 
	echo ""
	uname -a 
	echo "******************* dmesg ********************************"
	dmesg | tail -n 50
	echo "******************* ifconfig *****************************"
	ifconfig
	echo "******************* kernel cmd ***************************"
	cat  /proc/cmdline
	echo "******************* ANR LOGS *****************************"
	test -d /data/anr/ && for f in $( ls /data/anr/ -tr1 | tail -5 ); do test -f /data/anr/$f && cat /data/anr/$f ; done
	echo "******************* PROCESSES ****************************"
	ps
	echo "********************* TOP x 3 ****************************"
	top -m 20 -n 3 -b || top -m 20 -n 3
	echo "********************* df -h  *****************************"
	df -h 
	echo "******************** meminfo  ****************************"
	cat /proc/meminfo
	echo "******************** /proc/sys/fs/file-nr ****************"
	cat /proc/sys/fs/file-nr
	echo "**********************************************************"
}

function UploadLogs(){
	am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD --es ip_address ${LOG_ADDERSS} \
	--es post_data 'Group=DEFAULT&DeviceType=DEFAULT&DeviceID='$LOG_DEVICEID'&rawreport=' \
	--es post_path /cgi-bin/rawreport.cgi --es post_file ${TARGET_FILE}
}


if [ "$LOG_TYPE" = "0" ]; then
	logcat -d -T 1000 | tr -d ';&' > ${TARGET_FILE}
else
	PrintSysLogs | tr -d ';&' &>> ${TARGET_FILE}
fi

UploadLogs

test -f ${TARGET_FILE} && rm ${TARGET_FILE}
