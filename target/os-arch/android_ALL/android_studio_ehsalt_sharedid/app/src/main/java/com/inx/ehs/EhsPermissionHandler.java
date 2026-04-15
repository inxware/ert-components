package com.inx.ehs;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;

import com.inx.ehs.utils.EhsLogger;

import java.util.ArrayList;

public class EhsPermissionHandler {

    public static final int PERMISSIONS_REQUEST_ID = 1;

    // Permissions required for Marshmallow onwards
    private static final String [] PERMISSIONS_LIST = new String[]{
        Manifest.permission.WAKE_LOCK,
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE
    };

    private final Activity activity;

    public EhsPermissionHandler(Activity activity){
        this.activity = activity;
    }

    // Permissions required for Pie onwards
    private static final String [] PERMISSIONS_LIST_28 = new String[]{
    };

    public boolean request() {
        ArrayList<String> permissions = new ArrayList<String>();
        // Permissions required for Marshmallow onwards
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            for(String permission : PERMISSIONS_LIST) {
                if (activity.checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
                    permissions.add(permission);
                }
            }
        }
        // Permissions required for Pie onwards
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            for(String permission : PERMISSIONS_LIST_28) {
                if (activity.checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
                    permissions.add(permission);
                }
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !permissions.isEmpty()) {
            activity.requestPermissions(permissions.toArray(new String[0]), PERMISSIONS_REQUEST_ID);
            return true;
        }
        return false;
    }

    public boolean check(int requestCode, String[] permissions, int[] grantResults){
        try {
            if (requestCode == PERMISSIONS_REQUEST_ID) {
                if (permissions.length == grantResults.length) {
                    for (int i = 0; i < permissions.length; i++) {
                        String permission = permissions[i];
                        if (grantResults[i] == PackageManager.PERMISSION_GRANTED){
                            EhsLogger.info(permission + " ==========> Permission Granted");
                        }else{
                            EhsLogger.info(permission + " ==========> Permission NOT Granted !!!");
                            activity.finish();
                            return false;
                        }
                    }
                    return true;
                }
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }
}
