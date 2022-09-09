
using System;
using UnityEngine;

public class EHS_Utils
{
    public static string FixJson(string json){
        if(string.IsNullOrEmpty(json)){ 
            return "";    
        }
        string newjson = json.Replace("\"params\"", "\"parameters\"");
        newjson = newjson.Replace("\"smilText\"", "\"text\"");
        newjson = newjson.Replace("\n", "<p>");
        return newjson;    
    }

    public static string FixText(string text)
    {
        if(string.IsNullOrEmpty(text)){ 
            return "";    
        }
        string newText = text.Replace("<p>","\n");
        return newText;
    }

    public static void Log(string log){ 
        Debug.LogWarning(log);    
    }

    public static string ChangeAlpha(string colorStr, float alpha){
        string color = colorStr;
        try{
            string[] rgba = null;
            if(!string.IsNullOrEmpty(colorStr) && colorStr.Contains(".")){
                const float MAX_VALUE = 255.0f;
                rgba = colorStr.Split(char.Parse("."));
                if(rgba != null){
                    rgba[3] = (MAX_VALUE*alpha).ToString();
                    color = rgba[0] + "." + rgba[1] + "." + rgba[2] + "." + rgba[3];
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return color;
    }

    public static string AlphaChannel(string colorStr){
        string alpha = null;
        try{
            string[] rgba = null;
            if(!string.IsNullOrEmpty(colorStr) && colorStr.Contains(".")){
                
                rgba = colorStr.Split(char.Parse("."));
                if(rgba != null){
                    alpha = rgba[3];
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return alpha;
    }

    public static float Alpha(string colorStr){
        float alpha = 1f;
        try{
            const float MAX_VALUE = 255.0f;
            alpha = float.Parse(AlphaChannel(colorStr).ToString())/MAX_VALUE;
        }catch(Exception e){
            Debug.LogException(e);
        }
        return alpha;
    }

    public static Color PareseColor(string colorStr, Color defaultColor){ 
        Color color = defaultColor;
        try{
            string[] rgba = null;
            if(!string.IsNullOrEmpty(colorStr) && colorStr.Contains(".")){
                const float MAX_VALUE = 255.0f;
                rgba = colorStr.Split(char.Parse("."));
                if(rgba != null){ 
                    color.r = float.Parse(rgba[0])/MAX_VALUE;
                    color.g = float.Parse(rgba[1])/MAX_VALUE;
                    color.b = float.Parse(rgba[2])/MAX_VALUE;
                    color.a = float.Parse(rgba[3])/MAX_VALUE;
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
            color = defaultColor;
        }
        return color;
    }

    public static float Proc2Real(string proc){ 
        try{ 
            return float.Parse(proc.Split('%')[0])/100.0f;
        }catch(Exception e){ 
            Debug.LogException(e);
        }
        return 0f;
    }

    public static float PerspectiveHeight(float distance, float fieldOfView){ 
        return 2.0f * distance * Mathf.Tan(fieldOfView * 0.5f * Mathf.Deg2Rad);
    }

    public static float OrthographicHeight(float orthographicSize){ 
        return 2.0f * orthographicSize;
    }

    public static Camera View(){ 
        return Camera.main;
    }

    public static Vector3 FillScreen(){
        var view = View();
        float heigth;
        if(!view.orthographic){ 
            float distance = Mathf.Abs(view.transform.position.z);
            heigth = PerspectiveHeight(distance, view.fieldOfView);
            
        }else{
            heigth = OrthographicHeight(view.orthographicSize);
        }
        float width = heigth * Screen.width / Screen.height;
        return new Vector3(width, heigth, 0f);
    }

    public static Rect ZoneBounds(float top, float left, float bottom, float right){ 

        Vector3 fill = FillScreen();
        float x = (fill.x * left/2) - (fill.x * (1f - right)/2);
        float y = -(fill.y * top/2) + (fill.y * (1f - bottom)/2);
        float width = fill.x * (right - left);
        float height = fill.y * (bottom - top);

        return new Rect(x, y, width, height);
    }

    public static string[] ParseFlags(string flags){
        string[] flagsArray = null;
        if(!string.IsNullOrEmpty(flags)){
            if(flags.Contains("|")){
                flagsArray = flags.Split('|');
            }else{
                flagsArray = new string [1];
                flagsArray[0] = flags;
            }
        }
        return flagsArray;
    }

    public static bool Wait(int timeout, Func<bool> ready, int sleep = 500 /*[ms]*/){
        int attemptsCounter = timeout/sleep;
        do{ 
            if(ready()){ 
                return true;    
            }
            Debug.LogWarning("Wait number of attempts left ("+attemptsCounter+")");
            EHS_Runnable.Sleep(sleep);
            attemptsCounter--;
        } while(attemptsCounter > 0);
        return false;
    }
}
