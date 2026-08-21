package com.inx.ehssupervisor;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.content.Intent;
import android.os.Bundle;

import com.inx.ehssupervisor.utils.EHSS_Logger;

public class EHSS_HomeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Start service if it's not available
        if(EHSS_Manager.getInstance() == null){
            EHSS_Logger.info("Starting EHS Supervisor service from the UI Activity.");
            Intent service = new Intent(getApplicationContext(), EHSS_Service.class);
            ContextCompat.startForegroundService(getApplicationContext(), service);
        }
    }
}
