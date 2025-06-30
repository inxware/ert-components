package com.inx.ehs;

import android.os.Bundle;
import android.os.Environment;

import com.inx.ehs.utils.EhsLogger;
import com.inx.ehs.utils.EhsUtils;

public class EhsHomeNativeActivity extends EhsNativeActivity {

    private static final Integer MediaStorageRetries = 20;

    private boolean isStorageMounted() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

    private void waitForMediaStorageMounted() {
        try{
            Integer retries = 0;
            while(retries < MediaStorageRetries){
                if(isStorageMounted()){
                    EhsLogger.info("Media storage mounted!");
                    return;
                }
                EhsLogger.info("Media storage is not mounted yet (retries:"+retries+")");
                EhsUtils.sleep_ms(1000);
                retries++;
            }
        }catch(Exception e){
            EhsLogger.error(e.toString());    
        }
        EhsLogger.info("Failed to ensure that media storage has been mounted.");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        waitForMediaStorageMounted();
        EhsLogger.info("Starting Home App Activity ...");
        super.onCreate(savedInstanceState);
    }
}
