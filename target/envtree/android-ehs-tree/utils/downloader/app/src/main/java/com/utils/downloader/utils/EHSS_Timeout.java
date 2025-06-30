package com.utils.downloader.utils;

public class EHSS_Timeout extends EHSS_Timer {

    private long timeout = 0;

    public EHSS_Timeout(long timeout){
        this.timeout = (timeout < 0) ? 0 : timeout;
    }

    public void restart(){
        stop();
        start();
    }

    public boolean isTimeout(){
        return isStarted() && (elapsed() > timeout);
    }

}
