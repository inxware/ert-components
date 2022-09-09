package com.inx.ehssupervisor.utils;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.ResolveInfo;

import androidx.annotation.NonNull;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

public class EHSS_Utils {

    public static void sleep(int duration){
        try {
            Thread.sleep(duration);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void copyStream(InputStream input, OutputStream output) throws IOException {
        byte[] buffer = new byte[64*1024];
        int length;
        while ((length = input.read(buffer)) != -1) {
            output.write(buffer,0,length);
        }
    }

    public static boolean isInstalled(@NonNull Context context, String app){
        List<String> apps = getInstalledApps(context);
        if(!apps.isEmpty()){
            return apps.contains(app);
        }
        return false;
    }

    public static List<String> getInstalledApps(@NonNull Context context){

        List<String> ApkPackageName = new ArrayList<>();

        Intent intent = new Intent(Intent.ACTION_MAIN,null);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);

        List<ResolveInfo> resolveInfoList = context.getPackageManager().queryIntentActivities(intent,0);

        for(ResolveInfo resolveInfo : resolveInfoList){
            ActivityInfo activityInfo = resolveInfo.activityInfo;
            if(!isSystemPackage(resolveInfo)){
                ApkPackageName.add(activityInfo.applicationInfo.packageName);
            }
        }

        return ApkPackageName;
    }

    public static boolean isSystemPackage(ResolveInfo resolveInfo){
        return ( (resolveInfo.activityInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0 );
    }
}
