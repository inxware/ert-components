package com.inx.ehssupervisor;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;

import androidx.core.app.NotificationCompat;

import com.inx.ehssupervisor.utils.EHSS_Logger;

public class EHSS_Service extends Service {

    public static int SERVICE_NOTIFICATION_ID = 1;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        EHSS_Logger.info("EHS Supervisor Service ========> start command.");
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            /*Intent notificationIntent = new Intent(this, MainActivity.class);
              PendingIntent pendingIntent = PendingIntent.getActivity(this,
                    0, notificationIntent, 0); */

            Notification notification = new NotificationCompat.Builder(this, EHSS_App.SERVICE_CHANNEL_ID)
                    .setContentTitle("EHS Supervisor Service")
                    .setContentText("EHS Supervisor service is active.")
                    .setSmallIcon(R.drawable.ic_launcher_foreground)
                    //.setContentIntent(pendingIntent)
                    .build();

            startForeground(SERVICE_NOTIFICATION_ID, notification);
        }
        Thread managerThread = new Thread() {
            public void run() {
                runManager();
            }
        };
        managerThread.start();
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        EHSS_Logger.info("EHS Supervisor Service ========> destroyed.");
        super.onDestroy();
    }

    private void runManager(){
        EHSS_Logger.info("EHS Supervisor Service ========> run manager.");

        EHSS_Manager manager = EHSS_Manager.getInstance();
        if(manager == null){
            EHSS_Logger.info("EHS Supervisor Service ========> create manager.");
            if(!EHSS_Manager.createInstance(getApplicationContext())){
                EHSS_Logger.error("EHS Supervisor Service ========> failed to create manager.");
            }
        }
        manager = EHSS_Manager.getInstance();
        if(manager != null) {
            EHSS_Logger.info("EHS Supervisor Service ========> run manager.");
            manager.run(); // runs until signaled to stop
            EHSS_Manager.destroyInstance();
        }
    }
}
