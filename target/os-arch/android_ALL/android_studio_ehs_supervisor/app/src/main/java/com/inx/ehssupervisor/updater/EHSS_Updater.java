package com.inx.ehssupervisor.updater;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;

import androidx.annotation.NonNull;
import androidx.core.content.FileProvider;

import com.inx.ehssupervisor.EHSS_ActivityLauncher;
import com.inx.ehssupervisor.EHSS_Manager;
import com.inx.ehssupervisor.utils.EHSS_Logger;
import com.inx.ehssupervisor.utils.EHSS_Utils;
import com.inx.ehssupervisor.utils.comms.HttpServerRequest;
import com.inx.ehssupervisor.utils.comms.IMessage;
import com.inx.ehssupervisor.utils.comms.MessageFactory;
import java.io.File;

public abstract class EHSS_Updater {

    private final Context context;
    private final String address;

    public EHSS_Updater(@NonNull Context context, @NonNull String address){
        this.context = context;
        this.address = address;
    }

    public abstract String name();

    public abstract String packageName();

    public Context getContext() {
        return context;
    }

    public boolean download(){
        if(!address.isEmpty()){
            MessageFactory.JsonMessage request = new MessageFactory.JsonMessage(jsonString());
            HttpServerRequest serverRequest = HttpServerRequest.create(address, request);
            serverRequest.setExpected(MessageFactory.BIN_MESSAGE_TYPE);
            IMessage response = serverRequest.requestPost();
            if(response != null && response.isValid()){
                Integer type = response.getType();
                if(type != null && type.equals(MessageFactory.BIN_MESSAGE_TYPE)) {
                    if(((MessageFactory.BinMessage)response).save(downloadPath())){
                        return true;
                    }
                    EHSS_Logger.error("Download failed. Failed to save file to location (" +downloadPath()+ ").");
                }else{
                    EHSS_Logger.error("Download failed. Server response has an invalid type.");
                }
            }else{
                EHSS_Logger.error("Download failed. Server response is invalid.");
            }
        }
        return false;
    }

    public boolean install(){
        String filename = downloadPath();
        File file = new File(filename);
        if(file.exists()){
            try {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                Uri uri = FileProvider.getUriForFile(context, context.getApplicationContext().getPackageName() + ".provider", file);
                intent.setDataAndType(uri, "application/vnd.android.package-archive");
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                return EHSS_Manager.getInstance().launchActivity(new EHSS_ActivityLauncher(context, intent))
                        && EHSS_Utils.isInstalled(context, packageName());
            } catch (Exception e) {
                EHSS_Logger.debug(e.toString());
            }
        }
        return false;
    }

    private String jsonString() {
        return "{\"name\":\""+name()+"\"}";
    }

    private String downloadPath() {
        Context context = getContext();
        return context.getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS).toString() + "/"+name()+".apk";
    }
}
