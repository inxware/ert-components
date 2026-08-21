package com.inx.ehssupervisor.utils;

import com.inx.ehssupervisor.utils.EHSS_Timer;

public class EHSS_Timeout extends EHSS_Timer {

    private int timeout = 0;

    public EHSS_Timeout(int timeout){
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
