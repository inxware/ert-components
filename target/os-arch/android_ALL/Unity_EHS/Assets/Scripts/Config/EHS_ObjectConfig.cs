using System.Collections;
using System.Collections.Generic;
using UnityEngine;﻿

[System.Serializable]
public class EHS_ObjectConfig : EHS_Config
{
    public string parentID;
    public Vector3 position;
    public Vector3 scale;
    public Quaternion rotation;
    public bool active;
    public bool keep;
    public EHS_DetailConfig [] details;

    public static EHS_ObjectConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_ObjectConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_ObjectConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }

    public EHS_ObjectConfig appendDetail(EHS_DetailConfig config){
        List<EHS_DetailConfig> list;
        if(details != null){
            list = new List<EHS_DetailConfig>(details);
        }else{
            list = new List<EHS_DetailConfig>();
        }
        list.Add(config);
        details = list.ToArray();
        return this;
    }

    public static EHS_ObjectConfig Create(string id, string type, Vector3 position, Vector3 scale, Quaternion rotation, string parentID = null,
                                          EHS_DetailConfig [] details = null, bool keep = false)
    {
        EHS_ObjectConfig config = new EHS_ObjectConfig(type);
        config.id = id;
        config.position = position;
        config.scale = scale;
        config.rotation = rotation;
        config.keep = keep;
        config.details = details;
        config.parentID = parentID;
        return config;
    }

    public static EHS_ObjectConfig Create(string id, string type, string parentID = null, EHS_DetailConfig [] details = null,
                                          bool keep = false)
    {
        EHS_ObjectConfig config = Create(type, id, Vector3.zero, Vector3.one, Quaternion.identity);
        config.keep = keep;
        config.details = details;
        config.parentID = parentID;
        return config;
    }

    public EHS_ObjectConfig(string type) : base(type)
    {
        this.position = Vector3.zero;
        this.scale = Vector3.one;
        this.rotation = Quaternion.identity;
        this.active = true;
        this.keep = false;
        this.parentID = null;
    }

    public static void UpdateObject(EHS_Object ehsObject, EHS_ObjectConfig config)
    {
        if(!config.keep){
            GameObject unityObject = ehsObject.GetGameObject();
            if(config.position != null){
                unityObject.transform.localPosition = config.position;
            }
            if(config.rotation != null){
                unityObject.transform.localRotation = config.rotation;
            }
            if(config.scale != null){
                unityObject.transform.localScale = config.scale;
            }
        }
    }
}
