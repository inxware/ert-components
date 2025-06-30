package com.utils.downloader;

import android.app.Notification;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.ConditionVariable;
import android.os.IBinder;

import androidx.core.app.NotificationCompat;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

import java.io.File;
import java.util.HashMap;

public class DownloadService extends Service {

    public static int SERVICE_NOTIFICATION_ID = 1;

    private final HashMap<String, BroadcastReceiver> receivers = new HashMap<>();
    private static DownloadService instance = null;
    private DownloadRequest downloadRequest = null;

    public static int download(HashMap<String, String> extras){
        if(instance != null && instance.downloadRequest != null){
            if(!extras.isEmpty()) {
                instance.downloadRequest.Download(extras);
            }
            return DownloadRequest.DOWNLOADER_READY;
        }
        return DownloadRequest.DOWNLOADER_NOT_READY;
    }

    private void register(BroadcastReceiver receiver, String url){
        if(receiver != null){
            IntentFilter intentFilter = new IntentFilter(url);
            registerReceiver(receiver, intentFilter);
            receivers.put(url, receiver);
        }
    }

    private void unregister(){
        for(HashMap.Entry<String, BroadcastReceiver> entry : receivers.entrySet()){
            BroadcastReceiver receiver = entry.getValue();
            if(receiver != null) {
                unregisterReceiver(receiver);
                EHSS_Logger.info("unregister : " + entry.getKey());
            }
        }
        receivers.clear();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                Notification notification = new NotificationCompat.Builder(this, DownloaderApp.SERVICE_CHANNEL_ID)
                        .setContentTitle("Downloader Service")
                        .setContentText("Ready for downloading.")
                        .setSmallIcon(R.drawable.ic_launcher_foreground)
                        .build();
                startForeground(SERVICE_NOTIFICATION_ID, notification);
            }
            // unregister all broadcast receivers
            unregister();

            // register all broadcast receivers
            register(new DownloadReceiver(), "com.utils.downloader.broadcastreceiver.DOWNLOAD");
            register(new StatusReceiver(), "com.utils.downloader.broadcastreceiver.STATUS");
            register(new CertificateReceiver(), "com.utils.downloader.broadcastreceiver.CERTIFICATE");
            register(new UtilsReceiver(), "com.utils.downloader.broadcastreceiver.UTILS");

            instance = this;
            downloadRequest = new DownloadRequest();
            downloadRequest.Start();

            EHSS_Logger.info("======= Downloader Service [START] =======");
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EHSS_Logger.info("======= Downloader Service [STOP] =======");
        // unregister all broadcast receivers
        unregister();
        downloadRequest.Stop();
        downloadRequest = null;
        instance = null;
    }

    public class DownloadRequest extends Thread {

        public static final int DOWNLOADER_NOT_READY = 0;
        public static final int DOWNLOADER_READY = 100;
        public static final int DOWNLOAD_OK = 200;
        public static final int DOWNLOAD_FAIL = 404;
        public static final String DOWNLOAD_LOCK_FILE = "downloading.lock";

        private final ConditionVariable cv;
        private HashMap<String, String> extras;
        private boolean running;

        public DownloadRequest(){
            cv = new ConditionVariable();
            running = false;
            extras = null;
        }

        public void Start(){
            EHSS_Logger.info("======= Download Request Thread [START] =======");
            try {
                cv.close();
                synchronized (this) {
                    running = true;
                }
                start();
            }catch (Exception e){
                EHSS_Logger.error(e.toString());
            }
        }

        public void Stop(){
            try {
                synchronized (this) {
                    running = false;
                    extras = null;
                }
                cv.open();
                join();
            }catch (Exception e){
                EHSS_Logger.error(e.toString());
            }
            EHSS_Logger.info("======= Download Request Thread [STOP] =======");
        }

        public void Download(HashMap<String, String> extras){
            synchronized (this) {
                this.extras = extras;
            }
            cv.open();
        }

        public synchronized boolean isRunning(){
            return running;
        }

        @Override
        public void run() {
            while (isRunning()){
                cv.block();
                if(this.extras != null){
                    boolean success = Downloader.Exec(getApplicationContext(), extras);
                    synchronized (this) {
                        this.extras = null;
                        try {
                            // Define the path to the lock file
                            File lockFile = new File(getApplicationContext().getExternalFilesDir(null), DOWNLOAD_LOCK_FILE);
                            if (success) {
                                EHSS_Logger.info("Download Request Thread [SUCCESS].");
                                EHSS_Utils.write(lockFile, "result=" + DOWNLOAD_OK);
                            } else {
                                //EHSS_Logger.info("Download Request Thread [FAIL].");
                                EHSS_Utils.write(lockFile, "result=" + DOWNLOAD_FAIL);
                            }
                        }catch (Exception e){
                            EHSS_Logger.error("======= Download Request Thread [FAIL] =======. " + e.toString());
                        }
                    }
                }
                EHSS_Logger.info("======= Download Request Thread [OK] =======");
                cv.close();
            }
        }
    }
}
