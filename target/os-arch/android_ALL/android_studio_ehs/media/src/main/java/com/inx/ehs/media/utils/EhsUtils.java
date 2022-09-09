package com.inx.ehs.media.utils;

import android.content.Intent;
import com.inx.ehs.utils.EhsLogger;

public class EhsUtils {

    public static void appendIntentExtra(final Intent intent, String key, String value){
        try{
            String lcValue = value.toLowerCase();
            if(lcValue.equals("true")){
                intent.putExtra(key, true);
            }else if(lcValue.equals("false")){
                intent.putExtra(key, false);
            }else{
                intent.putExtra(key, value);
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
    }

    // handle spams
    private static final String FILE_PATH_SPAM="file:////";
    public static String removeSpam(String str){
        String outStr = str;
        // remove path type which is not supported by android

        if(outStr.contains(FILE_PATH_SPAM)){
            outStr = outStr.replace(FILE_PATH_SPAM, "/");
        }
        return outStr;
    }

}
