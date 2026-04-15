package com.inx.ehs.media.av;

import android.support.v4.app.INotificationSideChannel;

import androidx.annotation.NonNull;

import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;

public class AVAttributes {

    // attribute keys
    public static final String VOLUME = "volume";
    public static final String LENGTH = "length";
    public static final String TIME = "time";

    private final IEhsMediaPlayer mediaPlayer;

    public AVAttributes(@NonNull IEhsMediaPlayer mediaPlayer){
        this.mediaPlayer = mediaPlayer;
    }

    public String getString(String key){
        return null;
    }

    public Integer getInt(String key){
        try {
            if (VOLUME.equals(key)){
                return mediaPlayer.volume();
            }else if(LENGTH.equals(key)){
                return mediaPlayer.length();
            }else if(TIME.equals(key)){
                return mediaPlayer.time();
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return null;
    }
}
