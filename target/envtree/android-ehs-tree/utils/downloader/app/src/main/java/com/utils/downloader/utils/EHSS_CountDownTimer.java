package com.utils.downloader.utils;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;

public abstract class EHSS_CountDownTimer {

    private static final int MSG = 1;
    private final long timeout;
    private final long interval;
    private long stopTime;
    private boolean cancelled = false;
    private boolean running = false;

    public EHSS_CountDownTimer(long timeout, long interval) {
        this.timeout = timeout;
        this.interval = interval;
    }

    public synchronized final void cancel() {
        cancelled = true;
        running = false;
        handler.removeMessages(MSG);
    }

    public synchronized final EHSS_CountDownTimer start() {
        cancelled = false;
        if (timeout <= 0) {
            onTimeOut();
            return this;
        }
        stopTime = SystemClock.elapsedRealtime() + timeout;
        running = true;
        handler.sendMessage(handler.obtainMessage(MSG));
        return this;
    }

    public synchronized final void stop(){
        cancelled = true;
        running = false;
        handler.removeMessages(MSG);
        onStop();
    }

    public synchronized final boolean isRunning() {
        return running;
    }

    protected void resetTimer() {
        stopTime = SystemClock.elapsedRealtime() + timeout;
    }

    protected abstract void onTick(long millisUntilFinished);

    protected abstract void onTimeOut();

    protected void onStop(){}

    private Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {

            synchronized (EHSS_CountDownTimer.this) {
                if (cancelled) {
                    running = false;
                    return;
                }
                final long millisLeft = stopTime - SystemClock.elapsedRealtime();
                if (millisLeft <= 0) {
                    running = false;
                    onTimeOut();
                } else {
                    long lastTickStart = SystemClock.elapsedRealtime();
                    onTick(millisLeft);
                    long lastTickDuration = SystemClock.elapsedRealtime() - lastTickStart;
                    long delay;
                    if (millisLeft < interval) {
                        delay = millisLeft - lastTickDuration;
                        if (delay < 0) delay = 0;
                    } else {
                        delay = interval - lastTickDuration;
                        while (delay < 0) delay += interval;
                    }
                    sendMessageDelayed(obtainMessage(MSG), delay);
                }
            }
        }
    };
}
