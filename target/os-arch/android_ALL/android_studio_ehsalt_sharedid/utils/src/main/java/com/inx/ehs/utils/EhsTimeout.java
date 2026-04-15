package com.inx.ehs.utils;

public class EhsTimeout extends EhsTimer {

    public static EhsTimeout create(int timeout){
        EhsTimeout ehsTimeout = new EhsTimeout(timeout);
        ehsTimeout.start();
        return ehsTimeout;
    }

    private int timeout = 0;

    public EhsTimeout(int timeout){
        this.timeout = (timeout < 0) ? 0 : timeout;
    }

    public void restart(){
        stop();
        start();
    }

    public boolean isTimeout(){
        return isStarted() && (elapsed() > timeout);
    }

    public long remains(){
        long time_elapsed = elapsed();
        if(timeout > time_elapsed){
            return timeout - time_elapsed;
        }
        return 0;
    }

}
