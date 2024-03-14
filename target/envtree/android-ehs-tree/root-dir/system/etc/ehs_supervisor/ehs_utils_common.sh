#!/system/bin/sh

export EHS_REBOOT_FLAG="/data/local/tmp/ehs_reboot"

WriteRebootFlag(){
	touch "$EHS_REBOOT_FLAG"
}

ClearRebootFlag(){
	test -f "$EHS_REBOOT_FLAG" && rm "$EHS_REBOOT_FLAG"
}

IsRebootFlag(){
	test -f "$EHS_REBOOT_FLAG" && echo "yes"
}

WaitLockFile(){
	LOCK_FILE=$1
	D_RETRIES=$2 # seconds
	D_DELAY=1 # [sec]
	D_COUNTER=0
	D_RESULT="None"
	while [ "$D_COUNTER" -lt "$D_RETRIES" ]; do
		if [ -f "$LOCK_FILE" ]; then
			D_RESULT=$( cat $LOCK_FILE )
			rm $LOCK_FILE
			break
		fi
		D_COUNTER=$((D_COUNTER+1))
		sleep $D_DELAY
	done
	echo $D_RESULT
}

WaitDir(){
	DIR=$1
	RETRIES=$2
	DELAY=1 # [sec]
	COUNTER=0
	while [ "$COUNTER" -lt "$RETRIES" ]; do
		if [ -d "$DIR" ]; then
			echo "Exists"
			break			
		fi
		COUNTER=$((COUNTER+1))
		sleep $DELAY
	done
}

WaitFile(){
	FILE=$1
	RETRIES=$2
	DELAY=1 # [sec]
	COUNTER=0
	while [ "$COUNTER" -lt "$RETRIES" ]; do
		if [ -f "$FILE" ]; then
			echo "Exists"
			break			
		fi
		COUNTER=$((COUNTER+1))
		sleep $DELAY
	done
}

WaitFileGone(){
	FILE=$1
	RETRIES=$2
	DELAY=1 # [sec]
	COUNTER=0
	while [ "$COUNTER" -lt "$RETRIES" ]; do
		if ! [ -f "$FILE" ]; then
			echo "Gone"
			break			
		fi
		COUNTER=$((COUNTER+1))
		sleep $DELAY
	done
}

Unzip(){
	SOURCE=$1
	DEST=$2
	busybox unzip -q -o $SOURCE -d $DEST
}

Untar(){
	SOURCE=$1
	DEST=$2
	tar -C $DEST -xvf $SOURCE
}

InitDownloader(){
	DOWNLOADER_PACKAGE='com.utils.downloader'
	DOWNLOADER_SERVICE='.DownloadService'
	SERVICES=$( PackageServices ${DOWNLOADER_PACKAGE} )
	if [[ "$SERVICES" == *"$DOWNLOADER_SERVICE"* ]]; then
		echo "Download service is already running."
	else
		StartService ${DOWNLOADER_PACKAGE}/${DOWNLOADER_SERVICE}
		RETRIES=5; DELAY=2; COUNTER=0; SUCCESS=""; LOGGED="";
		while [ "$COUNTER" -lt "$RETRIES" ]; do
			TEST_BROADCAST=$( am broadcast -a com.utils.downloader.broadcastreceiver.DOWNLOAD )
			if [[ "$TEST_BROADCAST" == *"result=100"* ]]; then
				SUCCESS="TRUE"
				echo "Download service started."
				break			
			fi
			COUNTER=$((COUNTER+1))
			sleep $DELAY
		done
		if [ -z "$SUCCESS" ]; then 
			echo "Download Failed."
			StopService ${DOWNLOADER_PACKAGE}/${DOWNLOADER_SERVICE}
			sleep 1
			StopApp 'com.utils.downloader'
		fi
	fi
}

DownloaderStatus(){
	STATUS=$1
	if ! [ -z "$STATUS" ]; then
		MSG=$( InitDownloader )
		am broadcast -a com.utils.downloader.broadcastreceiver.STATUS --es 'current_status' "$STATUS"
	fi
}

DownloaderCommand(){
	COMMAND=$1
	if ! [ -z "$COMMAND" ]; then
		MSG=$( InitDownloader )
		am broadcast -a com.utils.downloader.broadcastreceiver.STATUS --es 'send_command' "$COMMAND"
	fi
}

LaunchDownloaderSetupPage(){
	PACKAGE="com.utils.downloader"
	INTENT=".InitActivity"
	RETRIES=10; DELAY=2; COUNTER=0;
	while [ "$COUNTER" -lt "$RETRIES" ]; do
		IS_HOME_APP=$( IsHomeApp $PACKAGE )
		if [ -z "$IS_HOME_APP" ]; then
			SetAsHomeApp $PACKAGE $INTENT
		fi
		sleep $DELAY
		IS_ACTIVE=$( IsActivityFocused $PACKAGE $INTENT )
		if [ "$IS_ACTIVE" = "YES" ]; then
			echo "Downloader setup page loaded."
			break			
		fi
		COUNTER=$((COUNTER+1))
	done
}

LaunchDownloaderUpdatePage(){
	PACKAGE="com.utils.downloader"
	INTENT=".UpdateActivity"
	RETRIES=5; DELAY=5; COUNTER=0;
	while [ "$COUNTER" -lt "$RETRIES" ]; do
		LaunchApp $PACKAGE $INTENT
		sleep $DELAY
		IS_ACTIVE=$( IsActivityFocused $PACKAGE $INTENT )
		if [ "$IS_ACTIVE" = "YES" ]; then
			echo "Updater setup page loaded."
			break			
		fi
		COUNTER=$((COUNTER+1))
	done
}

CloseDownloaderUpdatePage(){
	PACKAGE="com.utils.downloader"
	INTENT=".UpdateActivity"
	IS_ACTIVE=$( IsActivityFocused $PACKAGE $INTENT )
	if [ "$IS_ACTIVE" = "YES" ]; then
		DownloaderCommand "close"
	else
		echo "Downloader update page is not running."
	fi
}

RebootDevice(){
	#LaunchDownloaderUpdatePage
	#DownloaderStatus "System is rebooting ..."
	WriteRebootFlag
}

# read version code 
VersionCode(){
	PACKAGE=$1
	VERSION_CODE=$(dumpsys package $PACKAGE | grep "versionCode")
	if ! [ -z "$VERSION_CODE" ]; then
		sed 's/.*versionCode=\(.*\) minSdk.*/\1/' <<< "$VERSION_CODE"
	fi
}

RunningProcess(){
	NAME=$1
	echo $(ps -A -f | grep $NAME | awk '{print $2}' | xargs)
}

PackageServices(){
	PACKAGE=$1
	dumpsys activity services $PACKAGE
}

StopService(){
	SERVICE=$1
	echo "Stopping Service $SERVICE ..."
	am stopservice $SERVICE
}

StopApp(){
	PACKAGE=$1
	echo "Stopping $PACKAGE ..."
	am force-stop $PACKAGE
}

LaunchApp(){
	PACKAGE=$1
	INTENT=$2
	if ! [ -z "$INTENT" ]; then
		echo "Launching $PACKAGE ..."
		am start -n "$PACKAGE/$INTENT"
	else
		echo "Unable to launch the app ($PACKAGE) due to unspecified launcher intent."
	fi
}

InstallApp(){
	APK=$1
	echo "Installing $APK ..."
	pm install -g -r $APK
}

UninstallApp(){
	PACKAGE=$1
	echo "Uninstalling $PACKAGE ..."
	pm uninstall $PACKAGE
}

IsAppRunning(){
	PACKAGE=$1
	APP_PROCESS=$( pidof $PACKAGE )
	if ! [ -z "$APP_PROCESS" ]; then
		echo "YES"
	fi
}

LaunchHomeApp(){
	am start -a android.intent.action.MAIN -c android.intent.category.HOME
}

IsHomeApp(){
	PACKAGE=$1
	HOME_PROCESS=$( GetHomeApp )
	if [[ "$HOME_PROCESS" == *"$PACKAGE"* ]]; then
		echo "YES"
	fi
}

IsApkValid(){
	APK=$1
	VALID=$(busybox unzip -l $APK | grep .xml)
	if [ -z "$VALID" ]; then
		echo "invalid"
	fi	
}

IsPackageInstalled(){
	PACKAGE=$1
	INSTALLED=$( pm list packages | grep $PACKAGE )
	if ! [ -z "$INSTALLED" ]; then
		echo "YES"
	fi
}

GetFocusedActivity(){
	dumpsys window windows | grep -E 'mFocusedApp'
}

IsActivityFocused(){
	PACKAGE=$1
	INTENT=$2
	FOCUSED=$( GetFocusedActivity )
	if [[ "$FOCUSED" == *"$PACKAGE"* ]] && [[ "$FOCUSED" == *"$INTENT"* ]]; then
		echo "YES"
	fi
}

IsPackageFocused(){
	PACKAGE=$1
	FOCUSED=$( GetFocusedActivity )
	if [[ "$FOCUSED" == *"$PACKAGE"* ]]; then
		echo "YES"
	fi
}

IsBootComplete(){
	COMPLETE=$( getprop sys.boot_completed )
	if [ "$COMPLETE" = "1" ]; then
		echo "YES"
	fi
}

VolumeUp(){
	input keyevent 24
}

VolumeDown(){
	input keyevent 25
}

SetTimeZone(){
	setprop persist.sys.timezone "$1"
}

SetUTCDate(){
	UTCDATE=( $(echo "$1" | tr ' ') )
	MONTH=$( echo "" | awk '{print (index("JanFebMarAprMayJunJulAugSepOctNovDec","'${UTCDATE[1]}'")+2)/3 }' )
	if [ "$MONTH" -lt 10 ]; then
		MONTH="0$MONTH"
	fi
	DAY=${UTCDATE[2]}
	if [ "$DAY" -lt 10 ]; then
		DAY="0$DAY"
	fi
	YEAR=${UTCDATE[3]}
	UTCTIME=${UTCDATE[4]}
	UTCTIME=( $(echo "$UTCTIME" | tr ':' ' ') )
	NEWDATE=${MONTH}${DAY}${UTCTIME[0]}${UTCTIME[1]}${YEAR}'.'${UTCTIME[2]}
	date -u ${NEWDATE}
	hwclock --utc --systohc
}

SetNtpServer() { 
settings put global ntp_server $1 
}

GetTimeZone(){
	getprop persist.sys.timezone
}

GetUTCDate(){
	date -u
}

GetDate(){
	date
}

Locked(){
	LOCK_FILE=$1
	if [ -f "$LOCK_FILE" ]; then
		echo "YES"
	fi
}

Lock(){
	LOCK_FILE=$1
	IS_LOCKED=$( Locked "$LOCK_FILE" )
	if [ -z "$IS_LOCKED" ]; then
		echo "" > "$LOCK_FILE"
		if ! [ -f "$LOCK_FILE" ]; then
			echo "FAILED to create a lockfile ($LOCK_FILE)."	
		fi
	else
		echo "Already locked."	
	fi	 
}

Unlock(){
	LOCK_FILE=$1
	IS_LOCKED=$( Locked "$LOCK_FILE" )
	if [ "$IS_LOCKED" = "YES" ]; then
		rm "$LOCK_FILE"
		if [ -f "$LOCK_FILE" ]; then
			echo "FAILED to unlock ($LOCK_FILE)."	
		fi
	else
		echo "Already unlocked."	
	fi	 
}
