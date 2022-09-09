using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_AudioPresentation : EHS_Presentation
{
    protected override EHS_PresentationObject CreatePresentationObject(EHS_PresentationConfig config){ 
        return new AudioPresentationObject(config.id, this);     
    }

    public class AudioPresentationObject : EHS_PresentationObject {

        public AudioPresentationObject(string id, EHS_Presentation presentation) : base(id, EHS_PresentationType.Audio, EHS_ObjectType.Empty, presentation){
            
        }

        protected override void OnCreate(EHS_PresentationConfig config) {
            AddCommand(EHS_MediaSource.CreateCommand(GetParentID(), EHS_MediaSourceType.Audio, ""));
        }

        protected override void OnStop() {
            try{
                string id = GetParentID();
                var ehsObject = presentation.GetSceneManager().GetObject(id);
                var audioSource = ehsObject.GetMediaSourceHandler().GetMediaSource(EHS_MediaSourceType.Audio) as EHS_AudioSource;
                audioSource.Stop();
            }catch(System.Exception e){ 
                Debug.LogException(e);
            }
        }

        protected override void OnDestroy() {
            // do nothing
        }

        protected bool IsApplyingMedia(EHS_PresentationConfig config) {
            return !string.IsNullOrEmpty(config.type) && config.type.Equals(EHS_MediaSourceType.Audio) && !string.IsNullOrEmpty(config.path);
        }

        protected override void OnStartUpdate(EHS_PresentationConfig config){
            try{
                if(IsApplyingMedia(config)){
                    string id = GetParentID();
                    string path = config.path;
                    var ehsObject = presentation.GetSceneManager().GetObject(id);
                    var audioSource = ehsObject.GetMediaSourceHandler().GetMediaSource(EHS_MediaSourceType.Audio) as EHS_AudioSource;
                    audioSource.Play(path);
                }
            }catch(System.Exception e){ 
                Debug.LogException(e);
            }
        }
    }
}
