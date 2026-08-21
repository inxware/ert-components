package com.inx.ehssupervisor.utils;

import android.os.SystemClock;

public class EHSS_Timer {

    private long startTime = 0l;

    public EHSS_Timer start(){
        startTime = SystemClock.elapsedRealtime();
        return this;
    }

    public EHSS_Timer stop(){
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
