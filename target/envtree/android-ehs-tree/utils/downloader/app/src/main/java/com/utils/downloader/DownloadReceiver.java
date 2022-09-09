package com.utils.downloader;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.utils.downloader.utils.EHSS_Logger;
import java.util.HashMap;

public class DownloadReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        final HashMap<String, String> extras = Downloader.Intent2Extras(intent);
        if(extras != null) {
            int result = DownloadService.download(extras);
            setResult(result, null, null);
        }else{
            setResult(DownloadService.DownloadRequest.DOWNLOAD_FAIL, null, null);
            EHSS_Logger.error("Downloader failed! No arguments were passed.");
        }
    }
}
