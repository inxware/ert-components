package com.inx.ehssupervisor;

import android.content.Context;
import android.content.Intent;
import android.os.ConditionVariable;

import androidx.annotation.NonNull;
import com.inx.ehssupervisor.utils.EHSS_Logger;


public class EHSS_ActivityLauncher {

    private final Context context;
    private final Intent intent;
    private final ConditionVariable conditionVariable;
    private boolean valid;

    public EHSS_ActivityLauncher(@NonNull Context context, @NonNull Intent intent){
        this.context = context;
        this.intent = intent;
        this.conditionVariable = new ConditionVariable();
        this.valid = true;
    }

    public synchronized boolean isValid(){
        return valid;
    }

    public synchronized void setValid(boolean valid){
        this.valid = valid;
    }

    public void launch(){
        if(!isValid()){
            EHSS_Logger.error("Activity launcher INVALID!");
            return;
        }
        Intent intent = new Intent(context, EHSS_UIActivity.class);
        context.startActivity(intent);
    }

    public void waitLock(){
        if(!isValid()){
            EHSS_Logger.error("Activity launcher INVALID!");
            return;
        }
        EHSS_Logger.warning("[LOCK] Applying lock on the manager thread in order to launch external activity.");
        conditionVariable.close();
        conditionVariable.block();
        EHSS_Logger.warning("[UNLOCK] Activity launcher lock released.");
    }

    public void done(){
        if(!isValid()){
            EHSS_Logger.error("Activity launcher INVALID!");
            return;
        }
        conditionVariable.open();
    }

    Intent getIntent(){
        if(!isValid()){
            EHSS_Logger.error("Activity launcher INVALID!");
            return null;
        }
        return intent;
    }

}
