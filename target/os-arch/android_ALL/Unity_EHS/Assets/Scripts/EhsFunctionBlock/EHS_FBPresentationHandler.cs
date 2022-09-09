
using UnityEngine;

public class EHS_FBPresentationHandler : EHS_FunctionBlockObjectHandler
{
    private class MediaStateListener : IMediaStateListener
    {
        private EHS_FunctionBlockManager manager;

        public MediaStateListener(EHS_FunctionBlockManager manager)
        {
            this.manager = manager;
        }

        public void OnChanged(string id, string type)
        {
            var queue = manager.GetResponseQueue();
            queue.Push(new EHS_FBResponse(id, type));
        }

        public void OnFailed(string id, string type)
        {
            Debug.Log("Failed media state id("+id+"), type("+type+").");
        }
    }

    private class SceneManagerListener : ISceneManagerListener
    {
        private EHS_FunctionBlockManager manager;

        private EHS_CommandConfig config;

        public SceneManagerListener(EHS_FunctionBlockManager manager)
        {
            this.manager = manager;
        }

        public void OnCreated(EHS_Object ehsObject)
        {
            // Set media callback
            if(ehsObject != null && ehsObject.isParent()){
                ehsObject.SetCallBack(new MediaStateListener(manager));
            }
            // Unlock the function block manager
            manager.Unlock();
        }

        public void OnCreateFailed(string id)
        {
            // Unlock the function block manager
            manager.Unlock();
        }

        public bool OnDestroyed(string id)
        {
            if(config != null && config.objectConfig.id.Equals(id)){
                // recreate the object
                return false;
            }
            return true;
        }

        public void OnPostDestroy(string id){
            if(config != null && config.objectConfig.id.Equals(id)){
                manager.GetSceneManager().AddListener(id, new SceneManagerListener(manager));
                manager.AddSceneCommand(config);
            }else{
                Debug.LogWarning("Failed to re-create object ("+id+")");
                manager.Unlock();
            }
        }

        public SceneManagerListener SetCommandConfig(EHS_CommandConfig config){ 
            this.config = config;
            return this;
        }
    }

    public EHS_FBPresentationHandler(EHS_FunctionBlockManager manager) : base(manager)
    {
    }

    public override bool Create(EHS_FBObject ehsFbObject)
    {
        Debug.Log("Create Presentation Handler");
        EHS_FunctionBlockParametersConfig parameters = ehsFbObject.GetParameters();

        var command = new EHS_CommandConfig("createObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.Presentation);
        command.objectConfig.id = ehsFbObject.GetId();

        Rect zone = EHS_Utils.ZoneBounds(parameters.Top(), parameters.Left(), parameters.Bottom(), parameters.Right());

        // append detail
        command.objectConfig.appendDetail(new EHS_DetailConfig(parameters.type)
                            .setConfigID(EHS_PresentationHandler.CreateDetail)
                            .appendConfig(new EHS_PresentationConfig(parameters.type)
                            .SetZoneConfig(new EHS_BoundsConfig(zone))
                            .SetTextConfig(parameters.textConfig)
                            .SetFadeConfig(parameters.fadeConfig)
                            .SetCreateAnimationConfig(parameters.createAnimationConfig)
                            .SetDestoryAnimationConfig(parameters.destoryAnimationConfig)
                            .SetUpdateAnimationConfig(parameters.updateAnimationConfig)
                            .SetPosition(parameters.Position())
                            .SetScale(parameters.Scale())
                            .SetFlags(parameters.flags)
                            .SetBackgroundColor(parameters.bgrColor)
                            .SetForegroundColor(parameters.fgrColor)));
        command.objectConfig.position = new Vector3(zone.x, zone.y, 0f);

        // check if object already exists and create it again
        if(ehsFbObject.ReCreate()){ 
            Debug.Log("Re-Creating object ("+ehsFbObject.GetId()+").");
            var listeners = manager.GetSceneManager().GetListeners(ehsFbObject.GetId());
            foreach(var listener in listeners){
                if(listener != null && (listener is SceneManagerListener)){
                    ((SceneManagerListener)listener).SetCommandConfig(command);
                    manager.Lock();
                    Destroy(ehsFbObject);
                    return true;
                }
            }
            return false;
        }

        // set call backs for the presentation
        manager.GetSceneManager().AddListener(ehsFbObject.GetId(), new SceneManagerListener(manager));
        manager.Lock();
        manager.AddSceneCommand(command);
        return true;
    }

    public override void Destroy(EHS_FBObject ehsFbObject)
    {
        string command = EHS_ObjectUtils.DestroyObjectCommand(ehsFbObject.GetId());
        manager.AddSceneCommand(command);
    }

    public override bool SetMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config)
    {
        //Debug.Log("Set Media for Presentation Handler");
        EHS_SceneManager sceneManager = manager.GetSceneManager();
        var presentation = sceneManager.GetObject(ehsFbObject.GetId());
        if(presentation == null){
            Debug.LogWarning("Couldn't find content type for presentation object ("+ehsFbObject.GetId()+")");
            return false;    
        }
        // get content type
        string contentType = presentation.GetMetaData("contentType");
        // build set media command
        EHS_CommandConfig command = new EHS_CommandConfig("updateObject");
        // create object config
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.Presentation);
        command.objectConfig.id = ehsFbObject.GetId();
        command.objectConfig.keep = true;

        // create presentation for media
        var presentationConfig = new EHS_PresentationConfig(config.type)
                            .SetPath(config.path)
                            .SetText(config.text);
        // add parameters which can be updated when setting media
        var parameters = config.parameters;
        if(parameters != null){
            presentationConfig.SetTextConfig(parameters.textConfig);
            presentationConfig.SetUpdateAnimationConfig(parameters.updateAnimationConfig);
        }
        // append detail
        command.objectConfig.appendDetail(new EHS_DetailConfig(contentType)
                            .setConfigID(config.type)
                            .appendConfig(presentationConfig));
        manager.AddSceneCommand(command);
        return true;
    }

    public override bool StopMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config)
    {
        Debug.Log("Stop Media for Presentation Handler");
        string command = "{\"type\":\"stopObject\",\"objectConfig\":{\"id\":\"" + ehsFbObject.GetId() + "\",\"type\":\"presentation\"}}";
        manager.AddSceneCommand(command);
        return false;
    }
}
