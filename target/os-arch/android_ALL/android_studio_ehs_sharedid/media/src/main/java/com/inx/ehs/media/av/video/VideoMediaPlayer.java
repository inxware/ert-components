package com.inx.ehs.media.av.video;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.MediaController;
import android.widget.VideoView;

import androidx.annotation.NonNull;

import com.inx.ehs.media.av.AVConstants;
import com.inx.ehs.media.av.AVData;
import com.inx.ehs.media.av.EhsMediaHandler;
import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;
import com.inx.ehs.media.utils.EhsUtils;

import java.util.HashMap;

public class VideoMediaPlayer extends Activity implements IEhsMediaPlayer {

    public static final String TYPE = "video";

    private MediaController mediaController;
    private VideoView videoView;
    private String id;
    private boolean isLooping = false;

    public static void create(@NonNull Context context, HashMap<String, String> settings){
        Intent intent = new Intent(context, VideoMediaPlayer.class);
        if(settings != null){
            for(HashMap.Entry<String,String> entry : settings.entrySet()){
                EhsUtils.appendIntentExtra(intent, entry.getKey(), entry.getValue());
            }
        }
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();

        if(intent == null){
            EhsLogger.error("Intent must be specified for the the Video Player to work.");
            finish();
            return;
        }

        try {
            id = intent.getStringExtra(AVConstants.ID);
            if(id == null || id.isEmpty()){
                EhsLogger.error("Attempting to create Video Player without ID.");
                finish();
                return;
            }
            EhsMediaHandler.getInstance().update(id, this);
            videoView = new VideoView(this);
            videoView.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
            setContentView(videoView);

            boolean isUsingController = intent.getBooleanExtra(AVConstants.CONTROLLER, false);
            isLooping = intent.getBooleanExtra(AVConstants.LOOPING, false);
            String path = intent.getStringExtra(AVConstants.PATH);

            if (isUsingController) {
                mediaController = new MediaController(this);
                mediaController.setAnchorView(videoView);
                videoView.setMediaController(mediaController);
            } else {
                videoView.setMediaController(null);
            }

            Uri uri = Uri.parse(path);
            videoView.setVideoURI(uri);
            videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    EhsLogger.info("Video Player - track prepared.");
                    mp.setLooping(isLooping);
                    truckInfo(mp);
                }
            });
            videoView.setOnCompletionListener(
                new MediaPlayer.OnCompletionListener() {
                    @Override
                    public void onCompletion(MediaPlayer mp) {
                        EhsLogger.info("Video Player track completed.");
                    }
                }
            );
            videoView.setOnErrorListener(
                new MediaPlayer.OnErrorListener() {
                    @Override
                    public boolean onError(MediaPlayer mp, int what, int extra) {
                        EhsLogger.error("Video Player failed. Error code : ("+what+","+extra+")");
                        finish();
                        return false;
                    }
                }
            );
            videoView.requestFocus();
            videoView.start();
            EhsLogger.info("Video Player created.");
        }catch (Exception e){
            EhsLogger.error(e.toString());
            if(videoView != null) {
                videoView.stopPlayback();
            }
            finish();
        }
    }

    private static void truckInfo(MediaPlayer mp){
        EhsLogger.info("Video Player - track length : " + mp.getDuration());
        EhsLogger.info("Video Player - track info : ");
        for(MediaPlayer.TrackInfo info : mp.getTrackInfo()){
            EhsLogger.info(info.toString());
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(videoView != null){
            videoView.stopPlayback();
            videoView.suspend();
        }
        if(id != null && !id.isEmpty()){
            EhsMediaHandler.getInstance().remove(id);
        }
    }

    @Override
    public void destroy(){
        this.finish();
    }

    @Override
    public String type(){
        return TYPE;
    }

    @Override
    public boolean load(AVData loadData){
        try{
            String path = loadData.getPath();
            if(path != null && !path.isEmpty()) {
                videoView.stopPlayback();
                videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                    @Override
                    public void onPrepared(MediaPlayer mp) {
                        mp.setLooping(isLooping);
                        videoView.requestFocus();
                        videoView.start();
                        EhsLogger.info("Video Player track reloaded.");
                        truckInfo(mp);
                    }
                });
                Uri uri = Uri.parse(path);
                videoView.setVideoURI(uri);
                videoView.resume();
                return true;
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }

    @Override
    public boolean unload(){

        return true;
    }

    @Override
    public void play(){
        try {
            if(!videoView.isPlaying()) {
                videoView.start();
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    @Override
    public void stop(){
        pause();
    }

    @Override
    public void pause(){
        try {
            videoView.pause();
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    // @TODO - add volume control
    @Override
    public void volume(Integer value){

    }

    @Override
    public void time(Integer value) {  }

    @Override
    public Integer volume(){
        return null;
    }

    @Override
    public Integer length() { return null; }

    @Override
    public Integer time() { return null; }
}
