package com.inx.ehssupervisor.utils;

import android.util.Log;

public class EHSS_Logger {
    private static final String TAG = "EHS SUPERVISOR";

    public enum LogType{
        LOG_INFO,
        LOG_ERROR,
        LOG_WARNING,
        LOG_DEBUG
    }

    public static void log(LogType type, String text){
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
        log(LogType.LOG_INFO, text);
    }

    public static void error(String text){
        log(LogType.LOG_ERROR, text);
    }

    public static void warning(String text){
        log(LogType.LOG_WARNING, text);
    }

    public static void debug(String text){
        log(LogType.LOG_DEBUG, text);
    }
}
