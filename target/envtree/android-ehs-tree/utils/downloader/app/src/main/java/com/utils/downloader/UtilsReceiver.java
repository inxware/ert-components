package com.utils.downloader;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Volume;

public class UtilsReceiver extends BroadcastReceiver {

    private interface IUtilsReceiverCommand {
        void Run();
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        try {
            IUtilsReceiverCommand cmd = parseCommand(context, intent);
            if(cmd != null){
                cmd.Run();
            }
        } catch (Exception e) {
            EHSS_Logger.error("Failed UtilsReceiver : " + e);
        }
    }

    private IUtilsReceiverCommand parseCommand(Context context, Intent intent) {
        Integer get_master_volume = intent.getIntExtra("get_master_volume", 0);
        if(get_master_volume == 1){
            return new VolumeGetter(context);
        }
        Integer set_master_volume = intent.getIntExtra("set_master_volume", -1);
        if(set_master_volume > -1){
            return new VolumeSetter(context, set_master_volume);
        }
        return null;
    }

    private class VolumeGetter implements IUtilsReceiverCommand {
        private final Context context;
        public VolumeGetter(Context context){
            this.context = context;
        }
        @Override
        public void Run() {
            EHSS_Logger.info("Received utils request - Get Volume");
            Integer volume = EHSS_Volume.getMusicVolume(context);
            setResultData(volume.toString());
        }
    }

    private class VolumeSetter implements IUtilsReceiverCommand {
        private final Context context;
        private final Integer value;
        public VolumeSetter(Context context, int value){
            this.context = context;
            this.value = value;
        }
        @Override
        public void Run() {
            EHSS_Logger.info("Received utils request - Set Volume = " + value);
            EHSS_Volume.setMusicVolume(context, value);
        }
    }
}
