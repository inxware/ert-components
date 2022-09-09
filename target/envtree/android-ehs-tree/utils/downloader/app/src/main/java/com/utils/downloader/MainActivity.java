package com.utils.downloader;

import android.app.Activity;
import android.os.Bundle;
import androidx.annotation.Nullable;

import com.utils.downloader.utils.EHSS_Logger;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EHSS_Logger.info("======= Downloader MainActivity [START] =======");
        //@TODO - testing
        //startActivity( new Intent(MainActivity.this, InitActivity.class));
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        EHSS_Logger.info("======= Downloader MainActivity [STOP] =======");
    }
}
