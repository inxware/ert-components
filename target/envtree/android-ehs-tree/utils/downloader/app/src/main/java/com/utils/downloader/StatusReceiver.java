package com.utils.downloader;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.utils.downloader.utils.EHSS_Logger;

public class StatusReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
         try {
             EHSS_Logger.info("Received status request.");
             String status = intent.getStringExtra("current_status");
             if(status != null && !status.isEmpty()){
                DownloadBaseActivity.UpdateStatus(status);
                EHSS_Logger.info("Status : "+status);
             }
             String command = intent.getStringExtra("send_command");
             if(command != null && !command.isEmpty()){
                 DownloadBaseActivity.SendCommand(command);
                 EHSS_Logger.info("Command : "+command);
             }
        }catch(Exception e){
            EHSS_Logger.error(e.toString());
        }
    }
}
