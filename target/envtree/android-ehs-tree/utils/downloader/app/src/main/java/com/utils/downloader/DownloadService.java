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

import java.util.HashMap;

public class DownloadService extends Service {

    public static int SERVICE_NOTIFICATION_ID = 1;

    private final HashMap<String, BroadcastReceiver> receivers = new HashMap<>();
    private static DownloadService instance = null;
    private DownloadRequest downloadRequest = null;

    public static int download(HashMap<String, String> extras){
        if(instance != null && instance.downloadRequest != null){
            instance.downloadRequest.Download(extras);
            return instance.downloadRequest.Wait();
        }
        return 0;
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

        public static final int DOWNLOAD_OK = 200;
        public static final int DOWNLOAD_FAIL = 404;
        public static final int DOWNLOAD_TOO_LONG = 608;
        public static final int TOO_LONG_TIMEOUT = 40 * 1000;
        public static final String LONG_DOWNLOAD_LOCK_FILE = "/sdcard/.EHS/.longDownload";

        private final ConditionVariable cv, cv_d;
        private HashMap<String, String> extras;
        private boolean running;
        private int result;

        public DownloadRequest(){
            cv = new ConditionVariable();
            cv_d = new ConditionVariable();
            running = false;
            extras = null;
            result = 0;
        }

        public void Start(){
            EHSS_Logger.info("======= Download Request Thread [START] =======");
            try {
                cv.close();
                synchronized (this) {
                    running = true;
                    result = 0;
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
                this.result = 0;
                this.extras = extras;
            }
            cv.open();
        }

        public int Wait(){
            if(result == 0) {
                cv_d.close();
                if (!cv_d.block(TOO_LONG_TIMEOUT)) {
                    EHSS_Logger.info("======= Download Request Thread [TOO LONG] =======");
                    synchronized (this) {
                        result = DOWNLOAD_TOO_LONG;
                    }
                }
            }
            return result;
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
                        if(result == DOWNLOAD_TOO_LONG){
                            if(success){
                                EHSS_Logger.info("======= Download Request Thread [LONG WAIT SUCCESS] =======.");
                                EHSS_Utils.write(LONG_DOWNLOAD_LOCK_FILE, "result="+DOWNLOAD_OK);
                            }else{
                                EHSS_Logger.info("======= Download Request Thread [LONG WAIT FAIL] =======.");
                                EHSS_Utils.write(LONG_DOWNLOAD_LOCK_FILE, "result="+DOWNLOAD_FAIL);
                            }
                        }else{
                            if (success) {
                                result = DOWNLOAD_OK;
                            } else {
                                result = DOWNLOAD_FAIL;
                            }
                        }
                    }
                }
                EHSS_Logger.info("======= Download Request Thread [OK] =======");
                cv_d.open();
                cv.close();
            }
        }
    }
}
