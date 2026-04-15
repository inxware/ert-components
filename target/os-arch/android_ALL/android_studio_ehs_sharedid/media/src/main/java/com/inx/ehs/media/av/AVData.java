package com.inx.ehs.media.av;

import com.inx.ehs.media.interfaces.IData;
import com.inx.ehs.media.utils.EhsUtils;
import com.inx.ehs.utils.EhsLogger;

import java.util.HashMap;

public class AVData implements IData {

    private static final String AV_DATA_SPLIT = ",";
    private static final String AV_ENTRY_SPLIT = ":=";

    private final String data;

    public AVData(String data){
        this.data = EhsUtils.removeSpam(data);
    }

    public String getStringData(){
        return this.data;
    }

    public Integer getIntegerData(){
        try {
            return Integer.parseInt(data);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return null;
    }

    public HashMap<String, String> getMap(){
        try{
            HashMap<String, String> map = new HashMap<>();
            if(data.contains(AV_DATA_SPLIT)){
                String [] dataArray = data.split(AV_DATA_SPLIT);
                for(String entry : dataArray){
                    if(entry.contains(AV_ENTRY_SPLIT)){
                        String [] values = entry.split(AV_ENTRY_SPLIT);
                        map.put(values[0], values[1]);
                    }
                }
            }else {
                if(data.contains(AV_ENTRY_SPLIT)){
                    String [] values = data.split(AV_ENTRY_SPLIT);
                    map.put(values[0], values[1]);
                }else{
                    return null;
                }
            }
            return map;
        }catch (Exception e){}
        return null;
    }

    public String getPath(){
        HashMap<String, String> map = getMap();
        if(map != null && map.containsKey(AVConstants.PATH)){
            return map.get(AVConstants.PATH);
        }
        return this.data;
    }
}
