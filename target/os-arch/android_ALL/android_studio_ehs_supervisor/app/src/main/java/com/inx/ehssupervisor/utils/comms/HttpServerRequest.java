package com.inx.ehssupervisor.utils.comms;

import com.inx.ehssupervisor.utils.EHSS_Logger;

import java.io.BufferedWriter;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;

import javax.net.ssl.HttpsURLConnection;

public class HttpServerRequest extends ServerRequest {

    private static final int TIMEOUT = 5000;

    private String urlAddress = null;
    private Integer responseCode = null;

    public static HttpServerRequest create(String urlAddress, IMessage request){
        return new HttpServerRequest(urlAddress, request);
    }

    public HttpServerRequest(String urlAddress, IMessage request) {
        super(request);
        this.urlAddress = urlAddress;
    }

    public IMessage requestPost() {
        if(request == null || !request.isValid()){
            EHSS_Logger.error("Server request error: Invalid request.");
            return null;
        }
        IMessage response;
        try {
            URL url = new URL(urlAddress);
            URLConnection connection = url.openConnection();
            boolean isHttps = (connection instanceof HttpsURLConnection);

            if (isHttps) {
                ((HttpsURLConnection) connection).setRequestMethod("POST");
            } else {
                ((HttpURLConnection) connection).setRequestMethod("POST");
            }
            connection.setConnectTimeout(TIMEOUT);

            setupConnection(connection);

            connection.connect();

            if (isHttps) {
                responseCode = ((HttpsURLConnection) connection).getResponseCode();
            } else {
                responseCode = ((HttpURLConnection) connection).getResponseCode();
            }
            if(getExpected().equals(MessageFactory.OK_MESSAGE_TYPE) || getExpected().equals(EXPECTED_UNKNOWN)){
                response = new MessageFactory(MessageFactory.OK_MESSAGE_TYPE).create(null);
            }else {
                response = new MessageFactory(getExpected()).create(connection.getInputStream());
            }
        } catch (Exception e) {
            EHSS_Logger.debug("Server request error: " + e.toString());
            return null;
        }
        return response;
    }

    protected void setupConnection(URLConnection connection){
        try {
            HashMap<String, String> properties = request.getProperties();
            if(properties != null) {
                for (HashMap.Entry<String, String> entry : properties.entrySet()) {
                    connection.setRequestProperty(entry.getKey(), entry.getValue());
                }
            }
            OutputStream os = connection.getOutputStream();
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(os, "UTF-8"));
            writer.write(request.getString());
            writer.flush();
            writer.close();
            os.close();
        } catch (Exception e) {
            EHSS_Logger.debug("Server request error: " + e.toString());
        }
    }

    public Integer getResponseCode(){
        return responseCode;
    }
}
