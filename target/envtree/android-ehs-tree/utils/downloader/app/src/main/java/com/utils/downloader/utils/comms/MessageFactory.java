package com.utils.downloader.utils.comms;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.HashMap;

public class MessageFactory {

    public static final Integer EMPTY_MESSAGE_TYPE = 1;
    public static final Integer OK_MESSAGE_TYPE = 2;
    public static final Integer JSON_MESSAGE_TYPE = 3;
    public static final Integer BIN_MESSAGE_TYPE = 4;
    public static final Integer TEXT_MESSAGE_TYPE = 5;
    public static final Integer DEFAULT_MESSAGE_TYPE = 6;

    private final Integer type;

    public MessageFactory(Integer type){
        this.type = type;
    }

    public static class EmptyMessage implements IMessage{

        @Override
        public boolean isValid() {
            return true;
        }

        @Override
        public Integer getType() {
            return EMPTY_MESSAGE_TYPE;
        }

        @Override
        public String getString(){
            return "";
        }

        @Override
        public HashMap<String, String> getProperties() {
            return null;
        }
    }

    public static class OKMessage implements IMessage {

        @Override
        public boolean isValid() {
            return true;
        }

        @Override
        public Integer getType() {
            return OK_MESSAGE_TYPE;
        }

        @Override
        public String getString() {
            return "OK";
        }

        @Override
        public HashMap<String, String> getProperties() {
            return null;
        }
    }

    public static class DefaultMessage implements IMessage {

        public static String read(InputStream inputStream){
            String text = "";
            try {
                // Create buffered reader for the response stream
                BufferedReader reader = new BufferedReader(
                        new InputStreamReader(inputStream, "iso-8859-1"), 8);
                // Line from the input stream
                String line;
                // While there is still more response to read
                while ((line = reader.readLine()) != null) {
                    // Add line to jsonString
                    text += (line + "\n");
                }
            } catch (Exception e) {
                EHSS_Logger.error( "Failed to read text error: " + e.toString());
                return null;
            }
            return text;
        }

        private String text = null;
        private boolean valid = true;

        public DefaultMessage(InputStream stream){
            if(stream != null) {
                text = read(stream);
            }
        }

        public DefaultMessage(String text){
            this.text = text;
        }

        @Override
        public boolean isValid() {
            return valid;
        }

        @Override
        public Integer getType() {
            return DEFAULT_MESSAGE_TYPE;
        }

        @Override
        public String getString(){
            return text;
        }

        @Override
        public HashMap<String, String> getProperties() {
            return null;
        }
    }

    public static class JsonMessage implements IMessage {

        private String jsonString = null;
        private boolean valid = true;

        public JsonMessage(InputStream stream){
            if(stream != null) {
                jsonString = readJsonString(stream);
            }
            if(jsonString == null){
                valid = false;
            }
        }

        @Override
        public boolean isValid() {
            return valid;
        }

        @Override
        public Integer getType() {
            return JSON_MESSAGE_TYPE;
        }

        @Override
        public String getString(){
            return jsonString;
        }

        public JSONObject getJson(){
            try {
                // Create jsonObject from the jsonString and return it
                return new JSONObject(jsonString);
            } catch (JSONException e) {
                EHSS_Logger.error("Error parsing JSON data: " + e.toString());
            }
            return null;
        }

        public String readJsonString(InputStream inputStream){
            String jsonString = "";
            try {
                // Create buffered reader for the response stream
                BufferedReader reader = new BufferedReader(
                        new InputStreamReader(inputStream, "iso-8859-1"), 8);
                // Line from the input stream
                String line;
                // While there is still more response to read
                while ((line = reader.readLine()) != null) {
                    // Add line
                    jsonString += (line + "\n");
                }
            } catch (Exception e) {
                EHSS_Logger.error( "Failed to read JSON error: " + e.toString());
                return null;
            }
            return jsonString;
        }

        @Override
        public HashMap<String, String> getProperties() {
            HashMap<String, String> properties = new HashMap<>();
            properties.put("Content-Type", "application/json; charset=utf-8");
            return properties;
        }
    }

    public static class TextMessage implements IMessage {

        private String text = null;
        private boolean valid = true;

        public TextMessage(InputStream stream){
            if(stream != null) {
                text = DefaultMessage.read(stream);
            }
        }

        public TextMessage(String text){
            this.text = text;
        }

        @Override
        public boolean isValid() {
            return valid;
        }

        @Override
        public Integer getType() {
            return TEXT_MESSAGE_TYPE;
        }

        @Override
        public String getString(){
            return text;
        }

        @Override
        public HashMap<String, String> getProperties() {
            HashMap<String, String> properties = new HashMap<>();
            properties.put("Content-Type", "application/text; charset=utf-8");
            return properties;
        }
    }

    public static class BinMessage implements IMessage {

        private boolean valid = true;
        private final InputStream inStream;

        public BinMessage(InputStream stream){
            if(stream == null){
                valid = false;
            }
            this.inStream = stream;
        }

        @Override
        public boolean isValid() {
            return valid;
        }

        @Override
        public Integer getType() {
            return BIN_MESSAGE_TYPE;
        }

        @Override
        public String getString(){
            return "";
        }

        @Override
        public HashMap<String, String> getProperties() {
            return null;
        }

        public boolean save(File file){
            try {
                EHSS_Logger.info( "Saving binary to file (" +file.getAbsolutePath()+ ").");
                OutputStream outStream = new BufferedOutputStream(new FileOutputStream(file));
                EHSS_Utils.copyStream(inStream, outStream);
                outStream.close();
                return true;
            }catch (FileNotFoundException e){
                EHSS_Logger.debug(e.toString());
            }catch (IOException e){
                EHSS_Logger.debug(e.toString());
            }
            return false;
        }

        public boolean save(String path){
            try {
                return save(new File(path));
            }catch (Exception e){
                EHSS_Logger.debug(e.toString());
            }
            return false;
        }
    }

    public IMessage create(InputStream stream){
        IMessage message = null;
        if(type == JSON_MESSAGE_TYPE) {
            message = new JsonMessage(stream);
        }else if(type == TEXT_MESSAGE_TYPE) {
            message = new TextMessage(stream);
        }else if(type == DEFAULT_MESSAGE_TYPE){
            message = new DefaultMessage(stream);
        }else if(type == BIN_MESSAGE_TYPE){
            message = new BinMessage(stream);
        }else if(type == OK_MESSAGE_TYPE){
            message = new OKMessage();
        }else if(type == EMPTY_MESSAGE_TYPE){
            message = new EmptyMessage();
        }
        return message;
    }
}
