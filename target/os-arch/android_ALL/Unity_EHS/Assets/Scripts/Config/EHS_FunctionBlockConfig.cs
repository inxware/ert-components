using UnityEngine;﻿

[System.Serializable]
public class EHS_FunctionBlockConfig
{
    public string id;
    public string cmd;
    public string type;
    public string path;
    public string text;
    public EHS_FunctionBlockParametersConfig parameters;

    public static EHS_FunctionBlockConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_FunctionBlockConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_FunctionBlockConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }

    public static EHS_FunctionBlockConfig Create(string id, string cmd, string type, string path, string text,
                                                 EHS_FunctionBlockParametersConfig parameters)
    {
        EHS_FunctionBlockConfig functionBlock = new EHS_FunctionBlockConfig();
        functionBlock.id = id;
        functionBlock.cmd = cmd;
        functionBlock.type = type;
        functionBlock.path = path;
        functionBlock.text = text;
        functionBlock.parameters = parameters;
        return functionBlock;
    }

    public static EHS_FunctionBlockConfig Create(string id, string cmd)
    {
        return Create(id, cmd, null, null, null, null);
    }
}
