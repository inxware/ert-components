package com.utils.downloader.utils.comms;

import android.os.Build;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

import java.io.BufferedWriter;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSession;

public class HttpServerRequest extends ServerRequest {

    private static final int TIMEOUT = 30000;

    private String urlAddress = null;
    private Integer responseCode = null;

    public static HttpServerRequest create(String urlAddress, IMessage request){
        return new HttpServerRequest(urlAddress, request);
    }

    public static HttpServerRequest create(String urlAddress){
        return create(urlAddress, null);
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
                if(certificate != null){
                     verifier((HttpsURLConnection)connection, urlAddress);
                    ((HttpsURLConnection)connection)
                            .setSSLSocketFactory(certificate.getSSLContext().getSocketFactory());
                }
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
            EHSS_Logger.debug("Server request error (2): " + e.toString());
            return null;
        }
        return response;
    }

    public IMessage requestGet() {

        IMessage response;
        try {
            URL url = new URL(urlAddress);
            URLConnection connection = url.openConnection();
            boolean isHttps = (connection instanceof HttpsURLConnection);
            if (isHttps) {
                ((HttpsURLConnection) connection).setRequestMethod("GET");
                if(certificate != null){
                    EHSS_Logger.debug("Verifiying " + urlAddress);
                    verifier((HttpsURLConnection)connection, urlAddress);
                    ((HttpsURLConnection)connection)
                            .setSSLSocketFactory(certificate.getSSLContext().getSocketFactory());
                }
                else {
                    EHSS_Logger.debug("No Certificate found for " + urlAddress);
                }
            } else {
                ((HttpURLConnection) connection).setRequestMethod("GET");
            }
            connection.setConnectTimeout(TIMEOUT);
            connection.connect();
            if (isHttps) {
                responseCode = ((HttpsURLConnection) connection).getResponseCode();
                 EHSS_Logger.debug("Request returned with" + responseCode);
            } else {
                responseCode = ((HttpURLConnection) connection).getResponseCode();
            }
            if(getExpected().equals(MessageFactory.OK_MESSAGE_TYPE) || getExpected().equals(EXPECTED_UNKNOWN)){
                response = new MessageFactory(MessageFactory.OK_MESSAGE_TYPE).create(null);
            }else {
                response = new MessageFactory(getExpected()).create(connection.getInputStream());
            }
        } catch (Exception e) {
            EHSS_Logger.debug("Server request error 3: " + e.toString());
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
            EHSS_Logger.debug("Server request error 1: " + e.toString());
        }
    }

    public Integer getResponseCode(){
        return responseCode;
    }

    private void verifier(HttpsURLConnection client, final String url){
        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            return;
        }
        EHSS_Logger.debug("Verify url : " + url);
        client.setHostnameVerifier(new HostnameVerifier() {
            @Override
            public boolean verify(String hostname, SSLSession session) {
                //HostnameVerifier hv = HttpsURLConnection.getDefaultHostnameVerifier();
                return true;// hv.verify(verify_url, session);
            }
        });
    }
}
