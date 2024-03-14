#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"
# Override all of these function for a specific android version

ErrorUtilsOverride(){
	SupervisorError "Invalid ehs_utils.sh script. This script must be overrided for a specific android version."
	exit 1
}

WriteRebootFlag(){
	ErrorUtilsOverride
}

ClearRebootFlag(){
	ErrorUtilsOverride
}

IsRebootFlag(){
	ErrorUtilsOverride
}

WaitLockFile(){
	ErrorUtilsOverride
}

WaitDir(){
	ErrorUtilsOverride
}

WaitFile(){
	ErrorUtilsOverride
}

WaitFileGone(){
	ErrorUtilsOverride
}

Unzip(){
	ErrorUtilsOverride
}

Untar(){
	ErrorUtilsOverride
}

InitDownloader(){
	ErrorUtilsOverride
}

DownloaderStatus(){
	ErrorUtilsOverride
}

DownloaderCommand(){
	ErrorUtilsOverride
}

LaunchDownloaderSetupPage(){
	ErrorUtilsOverride
}

LaunchDownloaderUpdatePage(){
	ErrorUtilsOverride
}

CloseDownloaderUpdatePage(){
	ErrorUtilsOverride
}

RebootDevice(){
	ErrorUtilsOverride
}

VersionCode(){
	ErrorUtilsOverride
}

RunningProcess(){
	ErrorUtilsOverride
}

PackageServices(){
	ErrorUtilsOverride
}

StartService(){
	ErrorUtilsOverride
}

StopService(){
	ErrorUtilsOverride
}

StopApp(){
	ErrorUtilsOverride
}

LaunchApp(){
	ErrorUtilsOverride
}

InstallApp(){
	ErrorUtilsOverride
}

UninstallApp(){
	ErrorUtilsOverride
}

IsAppRunning(){
	ErrorUtilsOverride
}

LaunchHomeApp(){
	ErrorUtilsOverride
}

SetAsHomeApp(){
	ErrorUtilsOverride
}

GetHomeApp(){
	ErrorUtilsOverride
}

IsHomeApp(){
	ErrorUtilsOverride
}

IsApkValid(){
	ErrorUtilsOverride
}

IsPackageInstalled(){
	ErrorUtilsOverride
}

GetFocusedActivity(){
	ErrorUtilsOverride
}

IsActivityFocused(){
	ErrorUtilsOverride
}

IsPackageFocused(){
	ErrorUtilsOverride
}

IsBootComplete(){
	ErrorUtilsOverride
}

VolumeUp(){
	ErrorUtilsOverride
}

VolumeDown(){
	ErrorUtilsOverride
}

SetTimeZone(){
	ErrorUtilsOverride
}

GetTimeZone(){
	ErrorUtilsOverride
}

SetUTCDate(){
	ErrorUtilsOverride
}

GetUTCDate(){
	ErrorUtilsOverride
}

GetDate(){
	ErrorUtilsOverride
}

Locked(){
	ErrorUtilsOverride
}

Lock(){
	ErrorUtilsOverride
}

Unlock(){
	ErrorUtilsOverride
}

MountRW(){
	ErrorUtilsOverride
}

MountRO(){
	ErrorUtilsOverride
}
