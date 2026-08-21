package com.inx.ehssupervisor.configuration;

import org.json.JSONObject;

public class JSONConfiguration implements IConfiguration {

    private final JSONObject jsonObject;

    public JSONConfiguration(JSONObject jsonObject){
        this.jsonObject = jsonObject;
    }

    public JSONObject getJsonObject() { return jsonObject; }
}
