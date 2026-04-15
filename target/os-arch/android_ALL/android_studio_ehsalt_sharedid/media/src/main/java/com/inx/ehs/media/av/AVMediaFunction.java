package com.inx.ehs.media.av;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehs.media.interfaces.ICommand;
import com.inx.ehs.media.interfaces.IData;
import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;

import java.util.HashMap;

public class AVMediaFunction {

    private static final String FUNC_DESTROY = "destroy";
    private static final String FUNC_LOAD = "load";
    private static final String FUNC_UNLOAD = "unload";
    private static final String FUNC_PLAY = "play";
    private static final String FUNC_STOP = "stop";
    private static final String FUNC_PAUSE = "pause";
    private static final String FUNC_VOLUME = "volume";
    private static final String FUNC_TIME = "time";

    private final HashMap<String, AVCommand> commands;
    private final String id;
    private String commandName;
    private String data;
    private AVAttributes attributes;

    public AVMediaFunction(String id, @NonNull Context context) {
        this.id = id;
        this.commands = new HashMap<>();
        EhsMediaPlayer mediaPlayer = new EhsMediaPlayer(id, context);
        attributes = new AVAttributes(mediaPlayer);
        // register all commands
        commands.put(FUNC_DESTROY, new AVDestroyCommand(mediaPlayer));
        commands.put(FUNC_LOAD, new AVLoadCommand(mediaPlayer));
        commands.put(FUNC_UNLOAD, new AVUnloadCommand(mediaPlayer)); // needed ?
        commands.put(FUNC_PLAY, new AVPlayCommand(mediaPlayer));
        commands.put(FUNC_STOP, new AVStopCommand(mediaPlayer));
        commands.put(FUNC_PAUSE, new AVPauseCommand(mediaPlayer));
        commands.put(FUNC_VOLUME, new AVVolumeCommand(mediaPlayer));
        commands.put(FUNC_TIME, new AVTimeCommand(mediaPlayer));
    }

    public void setCommandName(String name){
        this.commandName = name;
    }

    public void setData(String data){
        this.data = data;
    }

    public void exec() {
        if(commandName != null){
            if(commands.containsKey(commandName)){
                ICommand command = commands.get(commandName);
                command.setData(new AVData(data));
                command.exec();
            }
        }
    }

    public void updateMediaPlayer(IEhsMediaPlayer mediaPlayer){
        if(mediaPlayer != null){
            for(AVCommand command : commands.values()){
                command.setMediaPlayer(mediaPlayer);
            }
            attributes = new AVAttributes(mediaPlayer);
        }
    }

    public AVAttributes attributes(){ return this.attributes; }

    private abstract class AVCommand implements ICommand {

        protected IEhsMediaPlayer mediaPlayer;
        protected AVData avData;

        public AVCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            this.mediaPlayer = mediaPlayer;
        }

        public void setMediaPlayer(@NonNull IEhsMediaPlayer mediaPlayer){
            this.mediaPlayer = mediaPlayer;
        }

        @Override
        public void setData(IData data) {
            if(data instanceof AVData) {
                this.avData = (AVData) data;
            }else {
                EhsLogger.error("Invalid media player data type.");
                this.avData = null;
            }
        }
    }

    /* Command used for loading media path */
    private class AVDestroyCommand extends AVCommand {

        public AVDestroyCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.destroy();
        }
    }

    /* Command used for loading media path */
    private class AVLoadCommand extends AVCommand {

        public AVLoadCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.load(avData);
        }
    }

    /* Command used for unloading media path */
    // @TODO - not implemented
    private class AVUnloadCommand extends AVCommand {

        public AVUnloadCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.unload();
        }
    }

    /* Command used for playing */
    private class AVPlayCommand extends AVCommand {

        public AVPlayCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.play();
        }
    }

    /* Command used for stopping */
    private class AVStopCommand extends AVCommand {

        public AVStopCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.stop();
        }
    }

    /* Command used for pausing */
    private class AVPauseCommand extends AVCommand {

        public AVPauseCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
        }

        @Override
        public void exec() {
            mediaPlayer.pause();
        }
    }

    /* Command used for setting volume */
    private class AVVolumeCommand extends AVCommand {

        private Integer volume;

        public AVVolumeCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
            this.volume = null;
        }

        @Override
        public void setData(IData data) {
            if(data != null && data instanceof AVData){
                volume = ((AVData)data).getIntegerData();
            }
        }

        @Override
        public void exec() {
            if(volume != null){
                mediaPlayer.volume(volume);
            }else{
                EhsLogger.error("Failed to set volume.");
            }
        }
    }

    /* Command used for setting media play time */
    private class AVTimeCommand extends AVCommand {

        private Integer time;

        public AVTimeCommand(@NonNull IEhsMediaPlayer mediaPlayer){
            super(mediaPlayer);
            this.time = null;
        }

        @Override
        public void setData(IData data) {
            if(data != null && data instanceof AVData){
                time = ((AVData)data).getIntegerData();
            }
        }

        @Override
        public void exec() {
            if(time != null){
                mediaPlayer.time(time);
            }else{
                EhsLogger.error("Failed to set time.");
            }
        }
    }
}
