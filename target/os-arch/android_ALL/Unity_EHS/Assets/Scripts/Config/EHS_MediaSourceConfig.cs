using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class EHS_MediaSourceConfig : EHS_Config
{
    public string path;
    public string handler = EHS_BasicTextureHandler.HandlerType;
    public string sides;
    // ...

    public EHS_MediaSourceConfig(string type) : base(type)
    {
    }

    public bool Equals(EHS_Config other)
    {
        EHS_MediaSourceConfig mediaSourceConfig = (EHS_MediaSourceConfig)other;
        if(mediaSourceConfig != null){ 
            return  mediaSourceConfig.id == this.id &&
                    mediaSourceConfig.type == this.type &&
                    mediaSourceConfig.path == this.path &&
                    mediaSourceConfig.handler == this.handler &&
                    mediaSourceConfig.sides == this.sides;
        }
        return false;    
    }

    public static EHS_MediaSourceConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_MediaSourceConfig>(jsonString);
    }

    public static EHS_MediaSourceConfig Create(string id, string type, string path, string handler, string sides)
    {
        EHS_MediaSourceConfig config = new EHS_MediaSourceConfig(type);
        config.id = id;
        config.type = type;
        config.path = path;
        config.handler = handler;
        config.sides = sides;
        return config;
    }

    public static EHS_MediaSourceConfig Create(string id, string type, string path)
    {
        return Create(id, type, path, EHS_BasicTextureHandler.HandlerType, null);
    }
}
