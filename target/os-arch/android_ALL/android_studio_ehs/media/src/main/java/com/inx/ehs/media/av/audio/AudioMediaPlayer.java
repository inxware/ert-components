package com.inx.ehs.media.av.audio;

import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.ConditionVariable;

import androidx.annotation.NonNull;

import com.inx.ehs.media.av.AVConstants;
import com.inx.ehs.media.av.AVData;
import com.inx.ehs.media.av.EhsMediaHandler;
import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;
import com.inx.ehs.utils.EhsTimeout;

import java.io.File;
import java.util.HashMap;

public class AudioMediaPlayer implements IEhsMediaPlayer {

    public static final String TYPE = "audio";

    private static final Integer UNDEFINED = -1;

    private final String id;
    private final Context context;
    private MediaPlayer mediaPlayer;
    private String lastPath;
    private Integer current_volume = 50; // 0-100
    private Integer duration = UNDEFINED;
    private Integer position = UNDEFINED;
    private InfoUpdateThread infoUpdateThread;

    public static void create(@NonNull Context context, HashMap<String, String> settings){
        String id =  settings.get(AVConstants.ID);
        AudioMediaPlayer player = new AudioMediaPlayer(context, id);
        EhsMediaHandler.getInstance().update(id, player);
        player.load(settings.get(AVConstants.PATH));
    }

    public AudioMediaPlayer(@NonNull Context context, String id){
        this.context = context;
        this.id = id;
        this.mediaPlayer = null;
        this.lastPath = "";
        this.infoUpdateThread = null;
    }

    @Override
    public String type(){
        return TYPE;
    }

    @Override
    public void destroy(){
        unload();
    }

    @Override
    public boolean load(AVData loadData){
        return load(loadData.getStringData());
    }

    public boolean load(final String fileName){
        try {
            if(mediaPlayer != null){
                stop();
            }
            mediaPlayer = MediaPlayer.create(context, Uri.fromFile(new File(fileName)));
            if(mediaPlayer != null) {
                mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                    @Override
                    public void onCompletion(MediaPlayer mp) {
                        try {
                            EhsMediaHandler.getInstance(context).mediaCallbacks().call(id, AVConstants.CALLBACK_PLAY_FINISHED);
                        }catch (Exception e){
                            EhsLogger.error(e.toString());
                        }
                    }
                });
                lastPath = fileName;
                setVolume(current_volume);
                infoUpdateThread = new InfoUpdateThread();
                infoUpdateThread.start();
                return true;
            }
        }catch (Exception e) {
            EhsLogger.error(e.toString());
        }
        lastPath = "";
        return false;
    }

    @Override
    public boolean unload(){
        if(!lastPath.isEmpty()) {
            stop();
            lastPath = "";
        }
        return true;
    }

    @Override
    public void play(){
        try {
            if(mediaPlayer != null){
                if(!mediaPlayer.isPlaying()) {
                    mediaPlayer.start();
                }
            }else{
                if(lastPath != null && !lastPath.isEmpty()) {
                    if(load(lastPath)) play();
                }
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    @Override
    public void stop(){
        try {
            infoUpdateThread.stopUpdate();
            synchronized (this) {
                duration = UNDEFINED;
                position = UNDEFINED;
            }
            if(mediaPlayer != null) {
                mediaPlayer.stop();
                mediaPlayer.release();
                mediaPlayer = null;
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    @Override
    public void pause(){
        try {
            if(mediaPlayer != null && mediaPlayer.isPlaying()) {
                mediaPlayer.pause();
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    private void setVolume(final int value){
        try {
            if (mediaPlayer != null) {
                float volume = (float) value / 100f;
                mediaPlayer.setVolume(volume, volume);
                EhsLogger.debug("Setting volume to [" + value + " %]");
                synchronized (AudioMediaPlayer.this) {
                    current_volume = value;
                }
            }
        } catch (Exception e) {
            EhsLogger.error(e.toString());
        }
    }

    @Override
    public void volume(Integer value) {
        if (value != null) {
            final Integer final_value = value;
            new Thread() {
                @Override
                public void run() {
                    setVolume(final_value);
                }
            }.start();
        }
    }

    @Override
    public Integer volume(){
        synchronized (this) {
            return this.current_volume;
        }
    }

    @Override
    public void time(Integer value) {
        if(value != null) {
            final Integer time_ms = value*1000;
            new Thread() {
                @Override
                public void run() {
                    EhsLogger.debug("Setting time to [" + time_ms + " ms]");
                    try {
                        if(mediaPlayer != null) {
                            mediaPlayer.seekTo(time_ms);
                        }
                    }catch (Exception e){
                        EhsLogger.error(e.toString());
                    }
                }
            }.start();
        }
    }

    @Override
    public Integer length() {
        synchronized (this) {
            return this.duration;
        }
    }

    @Override
    public Integer time() {
        synchronized (this) {
            return this.position;
        }
    }

    private void updateLength(){
        Integer duration = UNDEFINED;
        try {
            if(mediaPlayer != null) {
                duration = mediaPlayer.getDuration();
                if(duration > 0) {
                    duration = duration/1000;
                }
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        synchronized (this) {
            this.duration = duration;
        }
    }

    private void updatePosition(){
        Integer position = UNDEFINED;
        try {
            if(mediaPlayer != null) {
                position = mediaPlayer.getCurrentPosition();
                if(position > 0) {
                    position = position/1000;
                }
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        synchronized (this) {
            this.position = position;
        }
    }

    private class InfoUpdateThread extends Thread {

        private static final int TIMEOUT = 500;
        private boolean running = false;
        private ConditionVariable cv = new ConditionVariable();

        @Override
        public void run() {
            EhsLogger.info("Audio info thread [started]");
            setRunning(true);
            while (isRunning()){
                try {
                    EhsTimeout timeout = EhsTimeout.create(TIMEOUT);
                    cv.close();
                    if (length() == UNDEFINED) {
                        updateLength();
                    }
                    updatePosition();
                    if (timeout.isTimeout()) {
                        Thread.sleep(1);
                    } else {
                        long time_remains = timeout.remains();
                        if(time_remains > 0) {
                            cv.block(time_remains);
                        }else{
                            Thread.sleep(1);
                        }
                    }
                }catch (Exception e){
                    EhsLogger.error(e.toString());
                }
            }
        }

        public void stopUpdate(){
            try {
                setRunning(false);
                cv.open();
                this.join();
                EhsLogger.info("Audio info thread [stopped]");
            }catch (Exception e){
                EhsLogger.error(e.toString());
            }
        }

        private synchronized boolean isRunning(){
            return this.running;
        }

        private synchronized void setRunning(boolean running){
            this.running = running;
        }
    }
}
