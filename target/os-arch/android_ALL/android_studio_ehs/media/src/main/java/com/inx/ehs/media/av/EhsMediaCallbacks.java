package com.inx.ehs.media.av;

import com.inx.ehs.utils.EhsLogger;

import java.util.HashMap;
import java.util.Iterator;

public class EhsMediaCallbacks {

    private final HashMap<String, MediaCallback> mediaCallbackMap = new HashMap<>();

    public void register(String id, String callbackName, String dataPtr){
        try {
            String cId = callbackId(id, callbackName);
            if (!mediaCallbackMap.containsKey(cId)) {
                mediaCallbackMap.put(cId, new MediaCallback(id, callbackName, dataPtr));
            } else {
                EhsLogger.info("Media callback ("+cId+") already exists.");
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public void unregister(String id, String callbackName){
        try {
            String cId = callbackId(id, callbackName);
            if (mediaCallbackMap.containsKey(cId)) {
                mediaCallbackMap.remove(cId);
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public void unregister(String id){
        try {
            Iterator<String> it = mediaCallbackMap.keySet().iterator();
            while (it.hasNext()) {
                String key = it.next();
                if(id.equals(key.split("\\.")[0])){
                    it.remove();
                }
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public void call(String id, String callbackName){
        try {
            String cId = callbackId(id, callbackName);
            if (mediaCallbackMap.containsKey(cId)) {
                MediaCallback mediaCallback = mediaCallbackMap.get(cId);
                mediaCallback.call();
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    private String callbackId(String id, String callbackName){
        return id + "." + callbackName;
    }

    private class MediaCallback {
        public final String id;
        public final String callbackName;
        public final String dataPtr;

        public MediaCallback(String id, String callbackName, String dataPtr){
            this.id = id;
            this.callbackName = callbackName;
            this.dataPtr = dataPtr;
        }

        public void call(){
            EhsMediaHandler.jniCallback(id, callbackName, dataPtr);
        }
    }
}
