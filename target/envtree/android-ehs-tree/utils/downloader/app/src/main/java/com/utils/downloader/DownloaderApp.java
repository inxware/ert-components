package com.utils.downloader;

import android.app.Application;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.os.Build;

import com.utils.downloader.utils.EHSS_Logger;

public class DownloaderApp extends Application {

    public static final String SERVICE_CHANNEL_ID = "serviceChannelId";

    @Override
    public void onCreate() {
        super.onCreate();
        EHSS_Logger.info("======= Downloader Created =======");
        createNotificationChannel();
    }

    private void createNotificationChannel(){
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            NotificationChannel serviceChannel = new NotificationChannel(
                    SERVICE_CHANNEL_ID,
                    "Download Service Channel",
                    NotificationManager.IMPORTANCE_LOW //importance low should give silence
            );
            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(serviceChannel);
        }
    }
}
