using UnityEngine;﻿

[System.Serializable]
public class EHS_FunctionBlockParametersConfig
{
    public string type = EHS_PresentationType.Cube;

    // zone size
    public string top = "0%";
    public string left = "0%";
    public string bottom = "100%";
    public string right = "100%";

    // coords
    public float x = 0f;
    public float y = 0f;
    public float z = 0f;
    public float w = 1f;
    public float h = 1f;
    public float d = 1f;
    
    // colors
    public string bgrColor = "255.255.255.255";
    public string fgrColor = "255.255.255.255";

    // text config
    public EHS_TextConfig textConfig = new EHS_TextConfig();

    // fading config
    public EHS_FadeConfig fadeConfig = new EHS_FadeConfig();

    // animation configs
    public EHS_AnimationConfig createAnimationConfig;
    public EHS_AnimationConfig destoryAnimationConfig;
    public EHS_AnimationConfig updateAnimationConfig;

    public string flags = "";

    public static EHS_FunctionBlockParametersConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_FunctionBlockParametersConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_FunctionBlockParametersConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }

    public float Top(){ 
        return EHS_Utils.Proc2Real(top);
    }

    public float Left(){ 
        return EHS_Utils.Proc2Real(left);
    }

    public float Bottom(){ 
        return EHS_Utils.Proc2Real(bottom);
    }

    public float Right(){ 
        return EHS_Utils.Proc2Real(right);
    }

    public Vector3 Position(){ 
        return new Vector3(x, y, z);
    }

    public Vector3 Scale(){ 
        return new Vector3(w, h, d);
    }
}
