using UnityEngine;﻿

[System.Serializable]
public class EHS_CommandConfig : EHS_Config
{
    public EHS_ObjectConfig objectConfig;
    public EHS_EventConfig eventConfig;
    public EHS_MediaSourceConfig mediaSourceConfig;
    public EHS_FunctionBlockConfig functionBlockConfig;

    public EHS_CommandConfig(string type) : base(type)
    {
    }

    public static EHS_CommandConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_CommandConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_CommandConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }
}
