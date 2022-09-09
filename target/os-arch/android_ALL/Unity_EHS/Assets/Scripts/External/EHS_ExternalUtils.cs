using UnityEngine;

public class EHS_ExternalUtils
{
    private static readonly string AndroidUnityActivity = "com.unity3d.player.UnityPlayer";

    public static bool WriteExternal(string functionName, string id, string command, string data){ 
#if UNITY_ANDROID
        using (AndroidJavaClass unity_player = new AndroidJavaClass(AndroidUnityActivity))
        {
            AndroidJavaObject current_activity = unity_player.GetStatic<AndroidJavaObject>("currentActivity");
            current_activity.Call(functionName, id, command, data);
            if (AndroidJNI.ExceptionOccurred() != System.IntPtr.Zero){
                ExceptionError();
            }else{ 
                return true;    
            }
        }
#else
        Debug.LogError("Write External Media function is only available on Android devices.");
#endif
        return false;    
    }

    public static bool WriteExternal(string functionName, string id, string command){ 
        return WriteExternal(functionName, id, command, "");    
    }

    public static T ReadExternal<T>(string functionName, string id, string data){ 
#if UNITY_ANDROID
        using (AndroidJavaClass unity_player = new AndroidJavaClass(AndroidUnityActivity))
        {
            AndroidJavaObject current_activity = unity_player.GetStatic<AndroidJavaObject>("currentActivity");
            T response = current_activity.Call<T>(functionName, id, data);
            if (AndroidJNI.ExceptionOccurred() != System.IntPtr.Zero){
                ExceptionError();
            }else{ 
                return response;    
            }
        }
#else
        Debug.LogError("Write External Media function is only available on Android devices.");
#endif
        return default;    
    }

    public static T ReadExternal<T>(string functionName, string id){ 
        return ReadExternal<T>(functionName, id, ""); 
    }

    private static void ExceptionError(){ 
        Debug.LogError("Exception occurred while attempting to access external funtion.");
        AndroidJNI.ExceptionDescribe();
        AndroidJNI.ExceptionClear();
    } 
}
