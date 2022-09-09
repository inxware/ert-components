package com.inx.ehssupervisor.configuration;

import android.content.Context;
import android.content.SharedPreferences;

import androidx.annotation.NonNull;

public class EHSS_Configurations implements IConfiguration {

    public static final String EHSS_SETTINGS_KEY             = "ehssSettings";
    public static final String EHSS_SERVER_KEY               = "ehssServerKey";

    private final SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor = null;

    public EHSS_Configurations(@NonNull Context context, @NonNull String keyName){
        sharedPreferences = context.getSharedPreferences(keyName, Context.MODE_PRIVATE);
    }

    public EHSS_Configurations(@NonNull Context context){
        this(context, EHSS_SETTINGS_KEY);
    }

    private void beginWrite(){
        if(editor == null && sharedPreferences != null){
            editor = sharedPreferences.edit();
        }
    }

    private void endWrite(){
        if(editor != null){
            editor.apply();
            editor = null;
        }
    }

    public EHSS_Configurations write(){
        endWrite();
        return this;
    }

    public EHSS_Configurations setServerAddress(String value){
        beginWrite();
        if(editor != null){
            editor.putString(EHSS_SERVER_KEY, value);
        }
        return this;
    }

    public String getServerAddress() {
        String value = null;
        if(sharedPreferences != null) {
            value = sharedPreferences.getString(EHSS_SERVER_KEY, EHSS_Constants.SERVER_ADDRESS);
        }
        return value;
    }
}
