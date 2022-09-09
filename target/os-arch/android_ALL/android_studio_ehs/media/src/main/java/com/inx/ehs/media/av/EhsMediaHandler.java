package com.inx.ehs.media.av;

import android.content.Context;
import androidx.annotation.NonNull;
import com.inx.ehs.media.interfaces.IEhsMediaPlayer;
import com.inx.ehs.utils.EhsLogger;
import java.util.HashMap;

public class EhsMediaHandler {

    // Native Calls
    public static native void jniCallback(final String id, final String event, final String dataPtr);

    public interface IMediaRemoveListener{
        void onRemoved(String id);
    }

    private final Context context;
    private final HashMap<String, AVMediaFunction> functions;
    private final EhsMediaCallbacks callbacks;
    private IMediaRemoveListener removeListener = null;

    private static EhsMediaHandler s_instance = null;

    public static EhsMediaHandler getInstance(@NonNull Context context){
        if(s_instance == null){
            s_instance = new EhsMediaHandler(context);
        }
        return s_instance;
    }

    public static EhsMediaHandler getInstance(){
        return s_instance;
    }

    private EhsMediaHandler(@NonNull Context context){
        this.context = context;
        this.functions = new HashMap<>();
        this.callbacks = new EhsMediaCallbacks();
    }

    protected synchronized AVMediaFunction getMediaFunction(String id){ return functions.get(id); }

    protected synchronized void addMediaFunction(String id, AVMediaFunction function){ functions.put(id, function); }

    protected synchronized void removeMediaFunction(String id){ functions.remove(id); }

    public void process(final String id, final String commandName, final String data){
        try {
            if(!exists(id)) {
                EhsLogger.debug("Creating media player function ("+id+")");
                AVMediaFunction function = new AVMediaFunction(id, context);
                addMediaFunction(id, function);
            }
            AVMediaFunction function = getMediaFunction(id);
            function.setCommandName(commandName);
            function.setData(data);
            function.exec();
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public void update(final String id, IEhsMediaPlayer mediaPlayer){
        try {
            if(exists(id)) {
                EhsLogger.debug("Updating media player function ("+id+")");
                AVMediaFunction function = getMediaFunction(id);
                function.updateMediaPlayer(mediaPlayer);
            }else{
                EhsLogger.info("Unable to update Media Player for id:"+id);
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public void remove(final String id){
        try {
            if(exists(id)) {
                EhsLogger.debug("Removing media player function ("+id+")");
                AVMediaFunction function = getMediaFunction(id);
                function.updateMediaPlayer(null);
                removeMediaFunction(id);
                synchronized (this) {
                    if (removeListener != null) {
                        removeListener.onRemoved(id);
                    }
                }
                callbacks.unregister(id);
            }else{
                EhsLogger.info("Unable to remove id:"+id+" as it doesn't exists.");
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    public AVAttributes attributes(String id){
        try {
            if(exists(id)) {
                EhsLogger.debug("Updating media player function ("+id+")");
                AVMediaFunction function = getMediaFunction(id);
                return function.attributes();
            }else{
                EhsLogger.info("Unable to update Media Player for id:"+id);
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return null;
    }

    public synchronized boolean exists(final String id){
        try {
            return functions.containsKey(id);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }

    public synchronized void setRemoveListener(IMediaRemoveListener removeListener){
        this.removeListener = removeListener;
    }

    public synchronized boolean hasRemoveListener(){
        return this.removeListener != null;
    }

    public EhsMediaCallbacks mediaCallbacks(){ return this.callbacks; }

}
