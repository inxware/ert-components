using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class EHS_MediaSource
{
    protected EHS_Object ehsObject;

    public EHS_MediaSource(EHS_Object ehsObject)
    {
        this.ehsObject = ehsObject;
    }

    public abstract bool Create(EHS_MediaSourceConfig config);

    public abstract bool Attach(EHS_Object ehsObject, EHS_MediaSourceConfig config);

    public abstract bool Detach(EHS_Object ehsObject, EHS_MediaSourceConfig config);

    public abstract bool Destroy(EHS_MediaSourceConfig config);


    public static string CreateCommand(string id, string type, string path)
    {
        return "{\"type\":\"createMediaSource\",\"mediaSourceConfig\":{\"id\":\"" + id + "\",\"type\":\"" + type + "\",\"path\":\"" + path + "\"}}";
    }

    public static string SelectMedia(string name, string path){ 
        if(!string.IsNullOrEmpty(name) && name.Equals("media")){ 
            if(EHS_VideoSource.IsSupportedFile(path)){ 
                return EHS_MediaSourceType.Video;
            }else if(EHS_AudioSource.IsSupportedFile(path)){ 
                return EHS_MediaSourceType.Audio;
            }else{ 
                Debug.LogWarning("Unknown media source ("+path+")");
            }
        }
        return name;
    }

    public static bool IsSupportedMedia(string media){
        if(!string.IsNullOrEmpty(media)){ 
            return media.Equals(EHS_MediaSourceType.Video) || media.Equals(EHS_MediaSourceType.Image);
        }
        return false; 
    }
}
