package com.inx.ehs.utils;

import android.util.Log;

public class EhsLogger {

    public static final int LOG_DISABLE = -1;
    public static final int LOG_ERROR = 0;
    public static final int LOG_WARNING = 1;
    public static final int LOG_INFO = 2;
    public static final int LOG_DEBUG = 3;

    public static int LogLevel = LOG_DEBUG;

    private static final String TAG = "EHS JNI";

    public static void log(int type, String text){
        switch(type){
            case LOG_INFO:{
                Log.i(TAG, text);
                break;
            }
            case LOG_ERROR:{
                Log.e(TAG, text);
                break;
            }
            case LOG_WARNING:{
                Log.w(TAG, text);
                break;
            }
            case LOG_DEBUG:{
                Log.d(TAG, text);
                break;
            }
            default:{
                break;
            }
        }
    }

    public static void info(String text){
        if(LogLevel >= LOG_INFO){
            log(LOG_INFO, text);
        }
    }

    public static void error(String text){
        if(LogLevel >= LOG_ERROR) {
            log(LOG_ERROR, text);
        }
    }

    public static void warning(String text){
        if(LogLevel >= LOG_WARNING) {
            log(LOG_WARNING, text);
        }
    }

    public static void debug(String text){
        if(LogLevel >= LOG_DEBUG) {
            log(LOG_DEBUG, text);
        }
    }
}
