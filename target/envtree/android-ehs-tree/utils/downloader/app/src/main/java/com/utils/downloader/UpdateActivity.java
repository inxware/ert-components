package com.utils.downloader;


import android.os.Bundle;
import android.widget.TextView;
import com.utils.downloader.utils.EHSS_Logger;

public class UpdateActivity extends DownloadBaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setBaseActivity(this);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_update);
    }

    @Override
    public void onBackPressed() {
        EHSS_Logger.info("Back pressed on update.");
    }

    @Override
    protected void updateStatus(String status) {
        try {
            TextView textView = findViewById(R.id.textViewProgress);
            textView.setText(status);
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
    }

    @Override
    protected void handleCommand(String command) {
        try {
            if(command.equals("close")){
                finish();
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
    }

    @Override
    protected void onDestroy() {
        setBaseActivity(null);
        super.onDestroy();
    }
}