using System.Collections.Generic;

public class EHS_ConfigList
{
    public static string GetId(string type, string id){ 
        return (string.IsNullOrEmpty(id)) ? type : type + "_" + id;
    }

    public static string GetId(EHS_Config config){ 
        return GetId(config.type, config.id);
    }

    private Dictionary<string /*id*/, EHS_Config /*config*/> list = new Dictionary<string, EHS_Config>();

    public void Add(EHS_Config config){ 
        string id = GetId(config);
        if(!list.ContainsKey(id)){ 
            list.Add(id, config);
        }
    }

    public void Remove(string id){ 
        if(list.ContainsKey(id)){ 
            list.Remove(id);
        }
    }

    public EHS_Config GetConfig(string type, string id=null){
        string config_id = GetId(type, id);  
        if(list.ContainsKey(config_id)){ 
            return list[config_id];
        }
        return null;
    }

    public string GetStringConfig(string id=null, string def=""){
        string string_id = GetId(EHS_StringConfig.TypeName, id);  
        if(list.ContainsKey(string_id)){
            var config = list[string_id] as EHS_StringConfig;
            if(config != null){
                return config.value;
            }
        }
        return def;
    }

    public int GetIntConfig(string id=null, int def=0){ 
        string int_id = GetId(EHS_IntConfig.TypeName, id);
        if(list.ContainsKey(int_id)){
            var config = list[int_id] as EHS_IntConfig;
            if(config != null){
                return config.value;
            }
        }
        return def;
    }

    public float GetFloatConfig(string id=null, float def=0f){
        string float_id = GetId(EHS_FloatConfig.TypeName, id);
        if(list.ContainsKey(float_id)){
            var config = list[float_id] as EHS_FloatConfig;
            if(config != null){
                return config.value;
            }
        }
        return def;
    }

    public bool GetBoolConfig(string id=null, bool def=false){
        string bool_id = GetId(EHS_BoolConfig.TypeName, id);
        if(list.ContainsKey(bool_id)){
            var config = list[bool_id] as EHS_BoolConfig;
            if(config != null){
                return config.value;
            }
        }
        return def;
    }

    public int Count(){ 
        return list.Count;    
    }
}
