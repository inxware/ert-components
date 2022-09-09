package com.utils.downloader;

import androidx.appcompat.app.AppCompatActivity;

public abstract class DownloadBaseActivity extends AppCompatActivity {

    private static DownloadBaseActivity s_activity = null;

    public static synchronized void UpdateStatus(String status){
        if(s_activity != null){
            s_activity.updateStatus(status);
        }
    }

    public static synchronized void SendCommand(String command){
        if(s_activity != null){
            s_activity.handleCommand(command);
        }
    }

    protected synchronized static void setBaseActivity(DownloadBaseActivity activity){  s_activity = activity; }

    protected abstract void updateStatus(String status);

    protected abstract void handleCommand(String command);
}
