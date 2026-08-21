package com.inx.ehssupervisor.devman;

import com.inx.ehssupervisor.utils.EHSS_Logger;
import com.inx.ehssupervisor.utils.comms.HttpServerRequest;
import com.inx.ehssupervisor.utils.comms.IMessage;
import com.inx.ehssupervisor.utils.comms.MessageFactory;

import org.json.JSONObject;

public class EHSS_DevmanServer {

    private static final String END_POINT = "supervisor";

    private final String address;

    public EHSS_DevmanServer(String address){
        if(!address.endsWith("/")){
            this.address = address + "/" + END_POINT;
        }else {
            this.address = address + END_POINT;
        }
    }

    JSONObject post(JSONObject requestJson){
        EHSS_Logger.debug("Posting JSON to devman ("+((requestJson != null) ? requestJson.toString() : "null") +")");
        JSONObject responseJson = null;
        MessageFactory.JsonMessage request = new MessageFactory.JsonMessage(requestJson);
        HttpServerRequest serverRequest = HttpServerRequest.create(address, request);
        serverRequest.setExpected(MessageFactory.JSON_MESSAGE_TYPE);
        IMessage response = serverRequest.requestPost();
        if(response != null && response.isValid()){
            Integer type = response.getType();
            if(type != null && type.equals(MessageFactory.JSON_MESSAGE_TYPE)) {
                responseJson = ((MessageFactory.JsonMessage)response).getJson();
            }else{
                EHSS_Logger.error("Devman server response has an invalid type.");
            }
        }else{
            EHSS_Logger.error("Devman server response is invalid.");
        }
        return responseJson;
    }
}
