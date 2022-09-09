using System;
using System.Collections.Generic;
using UnityEngine;

public class EHS_VideoSource : EHS_MediaSource
{
    public static readonly string [] SUPPORTED_VIDEO_EXT =
        { "mp4","webm","mov","m4v"};
    
    public static bool IsSupportedFile(string path){ 
        foreach(var ext in SUPPORTED_VIDEO_EXT){ 
            if(path.Contains(ext)){ 
                return true;
            }
        }
        return false;
    }

    private Dictionary<string/*id*/, GameObject/*object*/> ehsVideoObjects;

    private EHS_TextureHandlers textureHandlers;

    public EHS_VideoSource(EHS_Object ehsObject) : base(ehsObject)
    {
        ehsVideoObjects = new Dictionary<string, GameObject>();
        textureHandlers = new EHS_TextureHandlers(this);
    }

    public bool Play(string path){ 
        if(ehsVideoObjects.ContainsKey(path)){ 
            GameObject unityObject = ehsVideoObjects[path];
            if(unityObject != null){
                var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                if(videoPlayer != null && !videoPlayer.IsPlaying()){
                    videoPlayer.Play();
                }
                return true;
            }
        }
        return false;
    }

    public bool Stop(string path){ 
        if(ehsVideoObjects.ContainsKey(path)){ 
            GameObject unityObject = ehsVideoObjects[path];
            if(unityObject != null){
                var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                if(videoPlayer != null && videoPlayer.IsPlaying()){
                    videoPlayer.Stop();
                }
                return true;
            }
        }
        return false;
    }

    public void StopAll(){
        try{
            foreach(var path in ehsVideoObjects.Keys){
                Stop(path);     
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
    }

    public override bool Create(EHS_MediaSourceConfig config)
    {
        try{
            var path = config.path;
            if(!ehsVideoObjects.ContainsKey(path)){
                GameObject prefab = ehsObject.GetManager().GetPrefab(config.type);
                if(prefab != null){
                    GameObject unityObject = GameObject.Instantiate(prefab);
                    if(unityObject != null){
                        var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                        videoPlayer.Initialise(textureHandlers, path);
                        ehsVideoObjects.Add(path, unityObject);
                        // attach video object to parent object
                        GameObject unityParnetObject = ehsObject.GetGameObject();
                        unityObject.transform.parent = unityParnetObject.transform;
                        unityObject.transform.position += unityParnetObject.transform.position;
                        return true;
                    }
                }
            }else{
                return Play(path);
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public override bool Attach(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
         try{
            var path = config.path;
            if(!ehsVideoObjects.ContainsKey(path)){
                if(!Create(config)){
                    return false;
                }
            }
            if(ehsVideoObjects.ContainsKey(path)){
                GameObject unityObject = ehsVideoObjects[path];
                if(unityObject != null){
                    var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                    if(videoPlayer != null){
                        videoPlayer.AddObject(ehsObject, config);
                        return true;
                    }
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public override bool Detach(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        foreach(var ehsVideoObject in ehsVideoObjects){
            var unityObject = ehsVideoObject.Value;
            if(unityObject != null){
                var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                if(videoPlayer != null){
                    videoPlayer.RemoveObject(ehsObject, config);
                }
            }
        }
        return true;
    }

    public override bool Destroy(EHS_MediaSourceConfig config)
    {
        try{
            if(ehsVideoObjects.ContainsKey(config.path)){
                GameObject unityObject = ehsVideoObjects[config.path];
                if(unityObject != null){
                    ehsVideoObjects.Remove(config.path);
                    var videoPlayer = unityObject.GetComponent<EHS_VideoPlayer>();
                    videoPlayer.Stop();
                    GameObject.Destroy(unityObject);
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }
}
