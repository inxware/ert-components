package com.inx.ehssupervisor.updater;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.apps.ambifier.EHSS_AmbifierUpdater;
import com.inx.ehssupervisor.apps.ehs.EHSS_EHSUpdater;
import com.inx.ehssupervisor.command.ICommand;
import com.inx.ehssupervisor.configuration.EHSS_Configurations;
import com.inx.ehssupervisor.configuration.IConfiguration;
import com.inx.ehssupervisor.configuration.JSONConfiguration;
import com.inx.ehssupervisor.utils.EHSS_Logger;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;

public class EHSS_UpdateCommand implements ICommand {

    public static final String NAME = "updater";
    private static final String END_POINT = "datarequest";

    private final Context context;
    private JSONObject jsonConfiguration;

    public EHSS_UpdateCommand(@NonNull Context context){
        this.context = context;
    }

    @Override
    public void configure(IConfiguration configuration) {
        if(configuration instanceof JSONConfiguration){
            jsonConfiguration = ((JSONConfiguration)configuration).getJsonObject();
        }else{
            EHSS_Logger.error("Invalid configuration type for the update command.");
        }
    }

    @Override
    public boolean exec() {
        HashMap<String, EHSS_Updater> updaters = getUpdaters();
        boolean success = true;
        for (HashMap.Entry<String, EHSS_Updater> entry : updaters.entrySet()) {
            String name = entry.getKey();
            EHSS_Updater updater = entry.getValue();
            EHSS_Logger.info("Starting update of [" +name+ "].");
            if(updater != null && updater.download()){
                EHSS_Logger.info("Successful download of updates for [" +name+ "].");
                if(updater.install()){
                    EHSS_Logger.info("Successful install of updates for [" +name+ "].");
                }else{
                    success = false;
                }
            }else{
                success = false;
            }
        }
        return success;
    }

    public HashMap<String, EHSS_Updater> getUpdaters(){
        if(jsonConfiguration == null){
            return null;
        }
        EHSS_Configurations configurations = new EHSS_Configurations(context);
        String serverAddress = configurations.getServerAddress();
        if(!serverAddress.endsWith("/")){
            serverAddress = serverAddress + "/" + END_POINT;
        }else{
            serverAddress = serverAddress + END_POINT;
        }
        // register app updaters
        HashMap<String, EHSS_Updater> updaters = new HashMap<>();
        if(jsonConfiguration.has(EHSS_EHSUpdater.NAME)) {
            updaters.put(EHSS_EHSUpdater.NAME, new EHSS_EHSUpdater(context, serverAddress));
        }
        if(jsonConfiguration.has(EHSS_AmbifierUpdater.NAME)) {
            updaters.put(EHSS_AmbifierUpdater.NAME, new EHSS_AmbifierUpdater(context, serverAddress));
        }
        // ...
        return updaters;
    }

    @Override
    public String id() {
        return NAME;
    }

    public static JSONObject createRequest(String appName){
        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put(appName, "yes");
            return jsonObject;
        }catch (JSONException e){
            EHSS_Logger.debug(e.toString());
        }
        return null;
    }
}
