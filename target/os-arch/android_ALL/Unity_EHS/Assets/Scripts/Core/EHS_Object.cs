using System.Collections.Generic;
using UnityEngine;

public interface IObjectListener
{
}

public interface IObjectUpdateListener : IObjectListener
{
    void OnUpdated(string type, EHS_Object ehsObject);
    void OnUpdateFail(string type, EHS_Object ehsObject);
}

public interface IObjectStopListener : IObjectListener
{
    void OnStop(EHS_Object ehsObject, string type, bool success);
}

public interface IObjectDestroyListener : IObjectListener
{
    void OnDestroy(EHS_Object ehsObject);
}

public interface IMediaStateListener : IObjectListener
{
    void OnChanged(string id, string type);
    void OnFailed(string id, string type);
}

public class EHS_Object
{
    private Dictionary<string, string> metaData;
    private EHS_SceneManager manager;
    private EHS_Object parent;
    private GameObject unityObject;
    private EHS_ObjectConfig config;
    private EHS_TextureContainer textureContainer;
    private EHS_MaterialContainer materialContainer;
    private EHS_MediaSourceHandler mediaSourceHandler;
    private IObjectUpdateListener objectUpdatelistener;
    private IObjectStopListener objectStopListener;
    private IObjectDestroyListener objectDestroylistener;
    private IMediaStateListener mediaStatelistener;
    private Dictionary<string,IBehaviourListener> behaviourListeners; 
    private bool isRoot;

    public EHS_Object(EHS_SceneManager manager, EHS_ObjectConfig config)
    {
        this.metaData = new Dictionary<string, string>();
        this.manager = manager;
        this.config = config;
        this.unityObject = null;
        this.isRoot = true;
        this.behaviourListeners = new Dictionary<string,IBehaviourListener>();
    }

    public GameObject Create(EHS_Object parent)
    {
        this.parent = parent;
        if(parent != null){
            this.textureContainer = parent.GetTextureContainer();
            this.materialContainer = parent.GetMaterialContainer();
            this.mediaSourceHandler = parent.GetMediaSourceHandler();
            this.isRoot = false;
        }else{
            this.textureContainer = new EHS_TextureContainer();
            this.materialContainer = new EHS_MaterialContainer();
            this.mediaSourceHandler = new EHS_MediaSourceHandler(this);
            this.isRoot = true;
        }
        GameObject prefab = GetManager().GetPrefab(config.type);
        if(prefab != null){
            unityObject = GameObject.Instantiate(prefab, config.position, config.rotation);
            if(unityObject != null){
                return unityObject;
            }
        }
        return null;
    }

    public void SetMetaData(string key, string value){ 
        if(metaData.ContainsKey(key)){ 
            metaData[key] = value;
            return;
        }
        metaData.Add(key, value);
    }

    public string GetMetaData(string key){ 
        if(metaData.ContainsKey(key)){ 
            return metaData[key];
        }
        return null;
    }

    public bool isParent()
    {
        return isRoot;
    }

    public EHS_Object GetParent()
    {
        return parent;
    }

    public EHS_SceneManager GetManager()
    {
        return manager;    
    }

    public EHS_ObjectConfig GetConfig()
    {
        return config;
    }

    public EHS_MediaSourceHandler GetMediaSourceHandler()
    {
        return mediaSourceHandler;
    }

    public string GetId()
    {
        return config.id;
    }

    public GameObject GetGameObject()
    {
        return unityObject;
    }

    public EHS_TextureContainer GetTextureContainer()
    {
        return textureContainer;
    }

    public EHS_MaterialContainer GetMaterialContainer()
    {
        return materialContainer;
    }

    public void Destroy()
    {
        if(objectDestroylistener != null){ 
            objectDestroylistener.OnDestroy(this);
        }
        EHS_MediaSourceHandler handler = GetMediaSourceHandler();
        handler.DetachAll(this);
        // Destroy material associated with this object
        EHS_ObjectUtils.DestroyMaterial(unityObject);
        if(isRoot){
            // destroy all children objects
            foreach(var ehsObject in GetChildren()){
                GetManager().DestroyObject(ehsObject.GetId());
            }
            // clear all textures
            textureContainer.Clear();
            // clear all materials
            materialContainer.Clear();
        }
    }

    public List<EHS_Object> GetChildren(){ 
        var list = new List<EHS_Object>();
        if(isParent()){
            GetManager().GetObjectChildren(GetId(), list);
        }
        return list;
    }

    public void UpdateState(string type, bool success)
    {
        if(objectUpdatelistener != null){
            if(success){
                objectUpdatelistener.OnUpdated(type, this);
            }else{
                objectUpdatelistener.OnUpdateFail(type, this);
            }
        }
    }

    public void Stop(string type, bool success){ 
        if(objectStopListener != null){
            objectStopListener.OnStop(this, type, success);
        }    
    }

    public void ChangeMediaState(string type, bool success)
    {
        if(mediaStatelistener != null){
            if(success){
                mediaStatelistener.OnChanged(GetId(), type);
            }else{
                mediaStatelistener.OnFailed(GetId(), type);
            }
        }
    }

    public void SetCallBack(IObjectListener listener)
    {
        if((listener as IObjectUpdateListener) != null){ 
            this.objectUpdatelistener = listener as IObjectUpdateListener;
        }else if((listener as IObjectDestroyListener) != null){
            this.objectDestroylistener = listener as IObjectDestroyListener;
        }else if((listener as IMediaStateListener) != null){
            this.mediaStatelistener = listener as IMediaStateListener;
        }else if((listener as IObjectStopListener) != null){
            this.objectStopListener = listener as IObjectStopListener;
        }else{
            Debug.Log("Attempt to set unknown object call back.");
        }
    }

    public IObjectDestroyListener GetDestroyListener(){ 
        return this.objectDestroylistener;    
    }

    public void SetBehaviourCallBack(string behaviour, IBehaviourListener listener)
    {
        if(listener == null){ 
            Debug.LogWarning("Failed to add listener ("+behaviour+"). Null cannot be set.");
            return;    
        }
        if(behaviourListeners.ContainsKey(behaviour)){
            behaviourListeners[behaviour] = listener;
        }else{
            behaviourListeners.Add(behaviour, listener);
        }
    }

    public IBehaviourListener GetBehaviourCallBack(string behaviour){
        if(behaviourListeners.ContainsKey(behaviour)){
            return behaviourListeners[behaviour];
        }
        return null;
    }
}
