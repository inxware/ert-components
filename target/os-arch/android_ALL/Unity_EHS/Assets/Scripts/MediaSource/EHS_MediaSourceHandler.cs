using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_MediaSourceHandler
{
    private Dictionary<string, EHS_MediaSource> mediaSources;

    public EHS_MediaSourceHandler(EHS_Object ehsObject)
    {
        mediaSources = new Dictionary<string, EHS_MediaSource>();
        // register media sources
        // (1) - Image source
        mediaSources.Add(EHS_MediaSourceType.Image, new EHS_ImageSource(ehsObject));
        // (2) - Video source
        mediaSources.Add(EHS_MediaSourceType.Video, new EHS_VideoSource(ehsObject));
        // (3) - Audio source
        mediaSources.Add(EHS_MediaSourceType.Audio, new EHS_AudioSource(ehsObject));
    }

    public bool Create(EHS_MediaSourceConfig config)
    {
        if(config != null){
            EHS_MediaSource source = mediaSources[config.type];
            if(source != null){
                return source.Create(config);
            }
        }
        return false;
    }

    public bool Destroy(EHS_MediaSourceConfig config)
    {
        if(config != null){
            EHS_MediaSource source = mediaSources[config.type];
            if(source != null){
                return source.Destroy(config);
            }
        }
        return false;
    }

    public bool Update(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        if(config != null){
            EHS_MediaSource source = mediaSources[config.type];
            if(source != null){
                return source.Attach(ehsObject, config);
            }
        }
        return false;
    }

    public bool DetachAll(EHS_Object ehsObject)
    {
        bool success = true;
        if(ehsObject != null){
            foreach(var media in mediaSources){
                var source = media.Value;
                if(source.Detach(ehsObject, null)){
                    success = true;
                }
            }
        }
        return success;
    }

    public EHS_MediaSource GetMediaSource(string type)
    {
        if(mediaSources.ContainsKey(type)){
            return mediaSources[type];
        }
        return null;
    }
}
