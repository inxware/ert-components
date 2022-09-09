using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

public class EHS_AudioSource : EHS_MediaSource {

    public static readonly string [] SUPPORTED_AUDIO_EXT = { "mp3", "wav", "ogg" };
    
    public static bool IsSupportedFile(string path){ 
        foreach(var ext in SUPPORTED_AUDIO_EXT){ 
            if(path.Contains(ext)){ 
                return true;
            }
        }
        return false;
    }

    private GameObject audioSourceObject;

    public EHS_AudioSource(EHS_Object ehsObject) : base(ehsObject){ 
        
    }

    public bool Play(string path){ 
        try{
            if(audioSourceObject != null){
                var audioPlayer = audioSourceObject.GetComponent<EHS_AudioPlayer>();
                if(audioPlayer != null){ 
                    audioPlayer.Play(path);
                    return true;
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public bool Stop(){ 

        try{
            if(audioSourceObject != null){
                var audioPlayer = audioSourceObject.GetComponent<EHS_AudioPlayer>();
                if(audioPlayer != null){ 
                    audioPlayer.Stop();
                    return true;
                }
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }


    public override bool Create(EHS_MediaSourceConfig config) {
        try{
            if(audioSourceObject == null){
                GameObject prefab = ehsObject.GetManager().GetPrefab(EHS_ObjectType.Audio);
                if(prefab != null){
                    audioSourceObject = GameObject.Instantiate(prefab);
                    if(audioSourceObject != null){
                        GameObject unityParnetObject = ehsObject.GetGameObject();
                        audioSourceObject.transform.parent = unityParnetObject.transform;
                        audioSourceObject.transform.position += unityParnetObject.transform.position;
                        return true;
                    }
                } 
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public override bool Destroy(EHS_MediaSourceConfig config) {
        throw new System.NotImplementedException();
    }

    public override bool Attach(EHS_Object ehsObject, EHS_MediaSourceConfig config) {
        return true;
    }

    public override bool Detach(EHS_Object ehsObject, EHS_MediaSourceConfig config) {
        return true;
    }
}
