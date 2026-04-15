package com.inx.ehs.utils;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.PowerManager;

public class EhsAppLock {

    private final Context context;

    private PowerManager.WakeLock wakeLock;
    private WifiManager.WifiLock  wifiLock;


    public EhsAppLock(final Context context){
        this.context = context;

        PowerManager powerManager = (PowerManager)context.getSystemService(context.POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, "ehs:full_wake");
        wakeLock.setReferenceCounted(false);
        WifiManager wm = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        wifiLock = wm.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "ehs:wifi");
        wifiLock.setReferenceCounted(false);
    }

    public void acquire(){
        wakeLock.acquire();
        wifiLock.acquire();
    }

    public void release(){
        wakeLock.release();
        wifiLock.release();
    }
}
