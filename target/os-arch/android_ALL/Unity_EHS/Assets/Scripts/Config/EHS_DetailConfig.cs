using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class EHS_DetailConfig : EHS_Config
{
    public EHS_ConfigList configs;

    public EHS_DetailConfig(string type) : base(type)
    {
    }

    public static EHS_DetailConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_DetailConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_DetailConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }

    public EHS_DetailConfig appendConfig(EHS_Config config){
        if(configs == null){
            configs = new EHS_ConfigList();    
        }
        configs.Add(config);
        return this;
    }

    public EHS_DetailConfig appendConfig(string id, EHS_Config config){
        if(!string.IsNullOrEmpty(id)){ 
            config.SetId(id);
        }
        return appendConfig(config);
    }

    public EHS_DetailConfig appendConfig(string id, string value){
        var config = new EHS_StringConfig(value);
        if(!string.IsNullOrEmpty(id)){ 
            config.SetId(id);
        }
        return appendConfig(config);
    }

    public EHS_DetailConfig appendConfig(string id, float value){
        var config = new EHS_FloatConfig(value);
        if(!string.IsNullOrEmpty(id)){ 
            config.SetId(id);
        }
        return appendConfig(config);
    }

    public EHS_DetailConfig appendConfig(string id, int value){
        var config = new EHS_IntConfig(value);
        if(!string.IsNullOrEmpty(id)){ 
            config.SetId(id);
        }
        return appendConfig(config);
    }

    public EHS_DetailConfig appendConfig(string id, bool value){
        var config = new EHS_BoolConfig(value);
        if(!string.IsNullOrEmpty(id)){ 
            config.SetId(id);
        }
        return appendConfig(config);
    }

    public EHS_DetailConfig setConfigID(string id){
        this.id = id;
        return this;
    }
}
