package com.inx.ehssupervisor;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.content.Intent;
import android.os.Bundle;

import com.inx.ehssupervisor.utils.EHSS_Logger;

public class EHSS_UIActivity extends AppCompatActivity {

    private static final int ACTIVITY_LAUNCHER_ID = 1001;
    private EHSS_ActivityLauncher launcher = null;

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

        EHSS_Manager manager = EHSS_Manager.getInstance();
        if(manager != null){
            launcher = manager.getActivityLauncher();
            if(launcher!= null) {
                Intent intent = launcher.getIntent();
                if (intent != null) {
                    startActivityForResult(intent, ACTIVITY_LAUNCHER_ID);
                    return;
                } else {
                    EHSS_Logger.error("Cannot launch null activity for result.");
                }
            }
            manager.blockAppFocus(true);
        }

        EHSS_Logger.info("Create EHS Supervisor UI.");
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode,resultCode,data);
        switch(requestCode) {
            case ACTIVITY_LAUNCHER_ID: {
                EHSS_Logger.info("Activity request completed with following result code " + resultCode + ".");
                if(launcher != null){
                    launcher.done();
                    finish();
                    return;
                }
                break;
            }
            default:{
                EHSS_Logger.error("Unknown activity request code: " + requestCode);
                break;
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        blockAppFocus(false);
    }

    @Override
    protected void onPause() {
        super.onPause();
        blockAppFocus(false);
    }

    @Override
    protected void onResume() {
        super.onResume();
        blockAppFocus(true);
    }

    private void blockAppFocus(boolean block){
        EHSS_Manager manager = EHSS_Manager.getInstance();
        if(manager != null){
            manager.blockAppFocus(block);
        }
    }
}
