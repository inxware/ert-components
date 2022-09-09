package com.inx.ehssupervisor;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import androidx.core.content.ContextCompat;

import com.inx.ehssupervisor.utils.EHSS_Logger;

public class EHSS_Boot extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        EHSS_Logger.info("Booting EHS Supervisor.");
        Intent service = new Intent(context, EHSS_Service.class);
        ContextCompat.startForegroundService(context, service);
    }
}
