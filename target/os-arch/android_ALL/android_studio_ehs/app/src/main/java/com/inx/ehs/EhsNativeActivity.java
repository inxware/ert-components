package com.inx.ehs;

import android.app.NativeActivity;
import android.content.Context;
import android.os.Bundle;

import com.inx.ehs.media.av.EhsMediaHandler;
import com.inx.ehs.utils.EhsLogger;
import com.inx.ehs.utils.EhsJNI;
import com.inx.ehs.utils.EhsAppLock;


public class EhsNativeActivity extends NativeActivity {

    private EhsAppLock ehsAppLock;

    @Override
    protected void onCreate(Bundle savedInstanceState){

        EhsLogger.LogLevel = EhsLogger.LOG_INFO;

        ehsAppLock = new EhsAppLock(this.getApplicationContext());
        ehsAppLock.acquire();

        EhsJNI.onCreate(this.getApplicationContext(), "EHS", "com.inx.ehs");
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onDestroy() {
        ehsAppLock.release();
        super.onDestroy();
    }

    public int JNI_AV_Command(final String id, final String commandName, final String data){
        try {
            EhsLogger.debug("JNI call : id (" + id + "), command (" + commandName + "), data (" + data + ")");
            Context context = EhsNativeActivity.this.getApplicationContext();
            EhsMediaHandler.getInstance(context).process(id, commandName, data);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return 0;
    }

    public int JNI_AV_RegisterCallback(final String id, final String callbackName, final String dataPtr){
        try {
            EhsLogger.debug("JNI register callback : id (" + id + "), callbackName (" + callbackName + "), dataPtr (" + dataPtr + ")");
            Context context = EhsNativeActivity.this.getApplicationContext();
            EhsMediaHandler.getInstance(context).mediaCallbacks().register(id, callbackName, dataPtr);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return 0;
    }

    public int JNI_AV_GetIntAttribute(final String id, final  String name){
        try {
            Context context = EhsNativeActivity.this.getApplicationContext();
            return EhsMediaHandler.getInstance(context).attributes(id).getInt(name);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return 0;
    }
}
