package com.inx.ehs.utils;

import android.os.SystemClock;

public class EhsTimer {

    private long startTime = 0l;

    public EhsTimer start(){
        startTime = SystemClock.elapsedRealtime();
        return this;
    }

    public EhsTimer stop(){
        startTime = 0l;
        return this;
    }

    public boolean isStarted(){
        return (startTime != 0l);
    }

    // returns elapsed time in milliseconds
    public long elapsed(){
        if(startTime == 0l) return 0l;
        return SystemClock.elapsedRealtime() - startTime;
    }
}
