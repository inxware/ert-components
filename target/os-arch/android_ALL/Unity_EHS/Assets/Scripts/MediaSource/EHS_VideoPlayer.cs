using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_VideoPlayer : MonoBehaviour
{
    private struct VideoPlayerDisplayObject
    {
        public EHS_Object ehsObject;
        public EHS_MediaSourceConfig config;

        public VideoPlayerDisplayObject(EHS_Object ehsObject, EHS_MediaSourceConfig config)
        {
            this.ehsObject = ehsObject;
            this.config = config;
        }
    }

    private List<VideoPlayerDisplayObject> displayObjects;
    private EHS_Object ehsParentObject;
    private UnityEngine.Video.VideoPlayer videoPlayer;
    private EHS_TextureHandlers textureHandlers;
    private bool isUpdateRequired = false;
    private string url;

    public void Initialise(EHS_TextureHandlers textureHandlers, string url)
    {
        this.textureHandlers = textureHandlers;
        this.url = url;
        displayObjects = new List<VideoPlayerDisplayObject>();
        isUpdateRequired = true;
    }

    // Start is called before the first frame update
    void Update()
    {
        if(isUpdateRequired){
            videoPlayer = GetComponent<UnityEngine.Video.VideoPlayer>();
            videoPlayer.url = url;
            videoPlayer.sendFrameReadyEvents = true;
            videoPlayer.frameReady += OnNewFarme;
            videoPlayer.loopPointReached += OnVideoFinished;
            videoPlayer.isLooping = true;
            videoPlayer.Prepare();
            videoPlayer.prepareCompleted += OnPrepared;
            isUpdateRequired = false;
        }
    }

    private void OnPrepared(UnityEngine.Video.VideoPlayer vp){
        Play();
    }

    public void AddObject(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        foreach(var displayObject in displayObjects){
            if(displayObject.ehsObject.GetId() == ehsObject.GetId() && config.Equals(displayObject.config)){
                Debug.Log("Object (" + ehsObject.GetId() + ") already exists in the list.");
                return;
            }
        }
        displayObjects.Add(new VideoPlayerDisplayObject(ehsObject, config));
        ehsParentObject = (ehsObject.isParent()) ? ehsObject : ehsObject.GetParent();
    }

    public void RemoveObject(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        if(config == null){
            displayObjects.RemoveAll(item => item.ehsObject.GetId() == ehsObject.GetId());
            return;
        }
        string sides = config.sides;
        for(int i = 0; i < displayObjects.Count; i++){
            var displayObject = displayObjects[i];
            if(displayObject.ehsObject.GetId() == ehsObject.GetId() && 
                (sides == null || sides.Equals(displayObject.config.sides))){
                displayObjects.RemoveAt(i);
                return;
            }
        }
    }

    public void Play()
    { 
        Debug.Log("Play video");
        videoPlayer.Play();
    }

    public void Stop()
    { 
        Debug.Log("Stop video");
        videoPlayer.Stop();
    }

    public Texture GetTexture()
    {
        if(videoPlayer != null){
            return videoPlayer.texture;
        }
        return null;
    }

    public bool IsPlaying(){ 
        return videoPlayer.isPlaying;   
    }

    private void OnVideoFinished(UnityEngine.Video.VideoPlayer player)
    {
        Debug.Log("Video Finished");
        if(ehsParentObject != null){
            ehsParentObject.ChangeMediaState("videoend", true);
        }
    }

    private void OnNewFarme(UnityEngine.Video.VideoPlayer player, long frameIndex)
    {
        //Debug.Log("Frame Index: texture" + frameIndex);
        if(textureHandlers != null){
            foreach(var displayObject in displayObjects){
               textureHandlers.Update(displayObject.ehsObject, displayObject.config, player.texture); 
            }
        }
    }
}
