using System;
﻿using System.Collections;
using System.IO;
using UnityEngine;
using UnityEngine.Networking;

public class EHS_ImageSource : EHS_MediaSource
{
    private EHS_TextureHandlers textureHandlers;
    private EHS_SceneManager sceneManager;

    public EHS_ImageSource(EHS_Object ehsObject) : base(ehsObject)
    {
        textureHandlers = new EHS_TextureHandlers(this);
        sceneManager = ehsObject.GetManager();
    }

    public override bool Create(EHS_MediaSourceConfig config)
    {
        try{
            var path = config.path;
            var textureContainer = ehsObject.GetTextureContainer();
            if(!textureContainer.Exists(path)){
                //return loadImage(path, textureContainer);
                return loadImageAsync(path, textureContainer);
            }else{
                //Debug.Log("Image texture (" + path + ") already exists.");
            }
        }catch(Exception e){
            // release any locks on the manager
            if(sceneManager != null){
                sceneManager.Unlock();
            }
            Debug.LogException(e);
        }
        return false;
    }

    private bool loadImage(string path, EHS_TextureContainer textureContainer){ 
        byte[] fileData = File.ReadAllBytes(path);
        if(fileData != null){
            Texture2D texture = new Texture2D(2,2);
            texture.wrapMode = TextureWrapMode.Clamp;
            texture.LoadImage(fileData);
            textureContainer.Add(path, texture);
            return true;
        }else{
            Debug.Log("Failed to read file (" + path + ").");
        }
        return false;
    }

    private bool loadImageAsync(string path, EHS_TextureContainer textureContainer){
        sceneManager.Lock();
        sceneManager.StartCoroutine(loadTextureAsync(path, textureContainer));
        return true;    
    }

    IEnumerator loadTextureAsync(string path, EHS_TextureContainer textureContainer)
    {
        string fullPath = "file://"+path;
        Debug.Log("FULL PATH: " + fullPath);
        using (UnityWebRequest uwr = UnityWebRequestTexture.GetTexture(fullPath))
        {
            yield return uwr.SendWebRequest();

            if (uwr.isNetworkError || uwr.isHttpError){
                Debug.Log(uwr.error);
            }else{
                // Get downloaded asset bundle
                var texture = DownloadHandlerTexture.GetContent(uwr);
                if(texture != null){
                    textureContainer.Add(path, texture);
                }else{ 
                    Debug.Log("Failed to load texture.");
                }
            }
            sceneManager.Unlock();
        }
    }

    public override bool Attach(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        try{
            var path = config.path;
            if(textureHandlers.Update(ehsObject, config)){
                return true;
            }
            return Create(config) && textureHandlers.Update(ehsObject, config);
        }catch(Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public override bool Detach(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        // no need to implement atm
        return true;    
    }

    public override bool Destroy(EHS_MediaSourceConfig config)
    {
        // @TODO
        return false;
    }
}
