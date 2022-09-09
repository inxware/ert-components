package com.inx.ehssupervisor.launcher;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.EHSS_Manager;
import com.inx.ehssupervisor.utils.EHSS_Logger;

import java.util.List;

public abstract class EHSS_Launcher {

    private final Context context;

    public EHSS_Launcher(@NonNull Context context){
        this.context = context;
    }

    public abstract String name();

    public abstract String packageName();

    public Context getContext() {
        return context;
    }

    public boolean isRunning(){
        return isAppRunning(context, packageName());
    }

    public boolean launch(){
        Intent launchIntent = context.getPackageManager().getLaunchIntentForPackage(packageName());
        if (launchIntent != null) {
            context.startActivity(launchIntent);
            return true;
        }
        return false;
    }

    public boolean focus(){
        if(EHSS_Manager.getInstance().isAppFocusBlocked()){
            EHSS_Logger.warning("Application focus has been suppressed.");
            return true;
        }
        Intent launchIntent = context.getPackageManager().getLaunchIntentForPackage(packageName());
        if (launchIntent != null) {
            launchIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
            context.startActivity(launchIntent);
            return true;
        }
        return false;
    }

    public boolean kill(){
        return false;
    }

    public boolean restart(){
        return kill() && launch();
    }

    public static boolean isAppRunning(final Context context, final String packageName) {
        // @TODO - this doesn't work atm, and is restricted to only showing this app
        final ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        final List<ActivityManager.RunningAppProcessInfo> processInfoList = activityManager.getRunningAppProcesses();
        if (processInfoList != null) {
            for (final ActivityManager.RunningAppProcessInfo processInfo : processInfoList) {
                if (processInfo.processName.equals(packageName)) {
                    return true;
                }
            }
        }
        return false;
    }
}
