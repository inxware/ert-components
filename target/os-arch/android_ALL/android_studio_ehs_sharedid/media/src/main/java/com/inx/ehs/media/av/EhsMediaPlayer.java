package com.inx.ehs.media.av;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehs.media.av.audio.AudioMediaPlayer;
import com.inx.ehs.media.av.video.VideoMediaPlayer;
import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;

import java.util.HashMap;

public class EhsMediaPlayer implements IEhsMediaPlayer {

    public static final String TYPE = "empty";

    public static final String [] SUPPORTED_AUDIO_TYPES = { "mp3", "wav", "ogg", "m4a", "aac", "flac", "3gp", "ts" };

    public static final String [] SUPPORTED_VIDEO_TYPES = { "mp4", "mov", "webm" };

    private final String id;
    private final Context context;

    public EhsMediaPlayer(String id, @NonNull Context context) {
        this.id = id;
        this.context = context;
    }

    @Override
    public String type(){
        return TYPE;
    }

    @Override
    public void destroy(){ /*do nothing*/ }

    @Override
    public boolean load(AVData loadData) {
        try {
            if (loadData != null) {
                String type = "";
                HashMap<String, String> settings = loadData.getMap();
                if (settings != null) {
                    if (!settings.containsKey(AVConstants.ID)) {
                        settings.put(AVConstants.ID, id);
                    }
                    if (!settings.containsKey(AVConstants.TYPE)) {
                        if(settings.containsKey(AVConstants.PATH)){
                            String path = settings.get(AVConstants.PATH);
                            type = findType(path);
                        }
                        if(type == null || type.isEmpty()) {
                            EhsLogger.warning("The type (audio or video) was not specified.");
                        }
                    }else{
                        type = settings.get(AVConstants.TYPE);
                    }
                } else {
                    String data = loadData.getStringData();
                    type = findType(data);
                    settings = new HashMap<String, String>();
                    settings.put(AVConstants.ID, id);
                    settings.put(AVConstants.TYPE, type);
                    settings.put(AVConstants.PATH, data);
                }
                if(type.equals(VideoMediaPlayer.TYPE)){
                    VideoMediaPlayer.create(context, settings);
                }else if(type.equals(AudioMediaPlayer.TYPE)){
                    AudioMediaPlayer.create(context, settings);
                }else{
                    EhsLogger.warning("Attempting to load unknown type.");
                    return false;
                }
                return true;
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }

    @Override
    public boolean unload() {
        requestWarning("UNLOAD");
        return true;
    }

    @Override
    public void play() {
        requestWarning("PLAY");
    }

    @Override
    public void stop() {
        requestWarning("STOP");
    }

    @Override
    public void pause() {
        requestWarning("PAUSE");
    }

    @Override
    public void volume(Integer value) {
        requestWarning("VOLUME");
    }

    @Override
    public void time(Integer value) { requestWarning("TIME"); }

    @Override
    public Integer volume() { return null; }

    @Override
    public Integer length() { return null; }

    @Override
    public Integer time() { return null; }

    private static void requestWarning(String request){
        EhsLogger.info("["+request+"] request on empty media player.");
    }

    private static String findType(String data){
        try {
            // Check if it's video
            for (String type : SUPPORTED_VIDEO_TYPES) {
                if(data.contains("."+type)){
                    return VideoMediaPlayer.TYPE;
                }
            }
            // Check if it's audio
            for (String type : SUPPORTED_AUDIO_TYPES) {
                if(data.contains("."+type)){
                    return AudioMediaPlayer.TYPE;
                }
            }
        }catch(Exception e){
            EhsLogger.error("Unable to find type : " + e.toString());
        }
        return "";
    }
}