package com.inx.ehssupervisor.devman;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.command.ICommand;
import com.inx.ehssupervisor.configuration.EHSS_Configurations;
import com.inx.ehssupervisor.configuration.IConfiguration;
import com.inx.ehssupervisor.configuration.JSONConfiguration;
import com.inx.ehssupervisor.updater.EHSS_UpdateCommand;
import com.inx.ehssupervisor.utils.EHSS_Logger;

import org.json.JSONException;
import org.json.JSONObject;


public class EHSS_DevmanServerPoll implements ICommand {

    private final String UPDATE_REQUEST_KEY = "update";
    private final Context context;

    public EHSS_DevmanServerPoll(@NonNull Context context){
        this.context = context;
    }

    @Override
    public void configure(IConfiguration configuration) {
        // do nothing
    }

    @Override
    public boolean exec() {
        EHSS_Logger.info("Polling from devman server.");
        EHSS_Configurations configurations = new EHSS_Configurations(this.context);
        String address = configurations.getServerAddress();

        EHSS_DevmanServer devmanServer = new EHSS_DevmanServer(address);
        JSONObject request = createRequest();
        JSONObject response = devmanServer.post(request);
        if(response != null) {
            // handle all responses
            // (1) Handle response where updates to the system were requested
            handleUpdateRequest(response);
            // (N) ...
            return true;
        }
        EHSS_Logger.error("Failed to get any responses from the devman server.");
        return false;
    }

    @Override
    public String id() {
        return "devmanPoll";
    }

    private void handleUpdateRequest(JSONObject response){
        try {
            JSONObject updateJson = null;
            String jsonString = response.getString(UPDATE_REQUEST_KEY);
            if(jsonString != null && !jsonString.isEmpty()){
                updateJson = new JSONObject(jsonString);
            }
            if(updateJson != null){
                EHSS_UpdateCommand updateCommand = new EHSS_UpdateCommand(context);
                updateCommand.configure(new JSONConfiguration(updateJson));
                updateCommand.exec();
            }
        }catch (JSONException e){
            EHSS_Logger.debug(e.toString());
        }
    }

    private static JSONObject createRequest(){
        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("", "");
            return jsonObject;
        }catch (JSONException e){
            EHSS_Logger.debug(e.toString());
        }
        return null;
    }
}
