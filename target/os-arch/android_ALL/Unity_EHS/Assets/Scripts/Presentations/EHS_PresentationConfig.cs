using UnityEngine;﻿
using System.Collections.Generic;

public class EHS_PresentationConfig : EHS_Config
{
    public EHS_TextConfig textConfig;
    public EHS_BoundsConfig zoneConfig;
    public EHS_FadeConfig fadeConfig;
    public EHS_AnimationConfig createAnimationConfig;
    public EHS_AnimationConfig destoryAnimationConfig;
    public EHS_AnimationConfig updateAnimationConfig;
    public Vector3 position;
    public Vector3 scale;
    public string path;
    public string text;
    public string bgrColor;
    public string fgrColor;
    public string [] flags;

    public EHS_PresentationConfig(string type) : base(type)
    {
    }

    public EHS_PresentationConfig SetTextConfig(EHS_TextConfig value){
        this.textConfig = value;
        return this;    
    }

    public EHS_PresentationConfig SetZoneConfig(EHS_BoundsConfig value){
        this.zoneConfig = value;
        return this;    
    }

    public EHS_PresentationConfig SetFadeConfig(EHS_FadeConfig value){
        this.fadeConfig = value;
        return this;    
    }

    public EHS_PresentationConfig SetCreateAnimationConfig(EHS_AnimationConfig value){
        if(value != null && value.type != null){
            this.createAnimationConfig = value;
        }else{ 
            this.createAnimationConfig = null;
        }
        return this;    
    }

    public EHS_PresentationConfig SetDestoryAnimationConfig(EHS_AnimationConfig value){
        if(value != null && value.type != null){
            this.destoryAnimationConfig = value;
        }else{
            this.destoryAnimationConfig = null;
        }
        return this;    
    }

    public EHS_PresentationConfig SetUpdateAnimationConfig(EHS_AnimationConfig value){
        if(value != null && value.type != null){
            this.updateAnimationConfig = value;
        }else{ 
            this.updateAnimationConfig = null;
        }
        return this;    
    }

    public EHS_PresentationConfig SetPosition(Vector3 value){
        this.position = value;
        return this;    
    }

    public EHS_PresentationConfig SetScale(Vector3 value){
        this.scale = value;
        return this;    
    }

    public EHS_PresentationConfig SetPath(string value){
        this.path = value;
        return this;    
    }

    public EHS_PresentationConfig SetText(string value){
        this.text = value;
        return this;    
    }

    public EHS_PresentationConfig SetBackgroundColor(string value){
        this.bgrColor = value;
        return this;    
    }

    public EHS_PresentationConfig SetForegroundColor(string value){
        this.fgrColor = value;
        return this;    
    }

    public EHS_PresentationConfig SetFlags(string value){
        this.flags = EHS_Utils.ParseFlags(value);
        return this;    
    }
    
    public static EHS_PresentationConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_PresentationConfig>(jsonString);
    }

    public static string CreateFromConfig(EHS_PresentationConfig config){ 
        return JsonUtility.ToJson(config);
    }

    public string ToJSON(){ 
        return CreateFromConfig(this);    
    }

    public EHS_PresentationConfig Copy(){   
        return (EHS_PresentationConfig)this.MemberwiseClone();
    }

    public bool IsFlag(string flag){ 
        if(!string.IsNullOrEmpty(flag) && flags != null){
            foreach(var f in flags){
                if(f.Contains(flag)){
                    return true;
                }
            }
        }
        return false;
    }

    public void AppendFlag(string flag){
        List<string> list;
        if(flags != null){
            list = new List<string>(flags);
        }else{
            list = new List<string>();
        }
        list.Add(flag);
        flags = list.ToArray();
    }

    public bool IsTextOnly(){ 
        return type == EHS_ObjectType.Text;    
    }
}
