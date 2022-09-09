package com.utils.downloader;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.EHSS_Utils;

public class InitActivity extends DownloadBaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setBaseActivity(this);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_init);
        try {
            Button button = findViewById(R.id.button);
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    EHSS_Utils.launchSettings(InitActivity.this);
                }
            });
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
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