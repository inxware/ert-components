using System;
using System.Collections.Generic;
using UnityEngine;

public abstract class EHS_PresentationObject
{
    private EHS_Animation.ICompleteListener destroyAnimationCompletedListener;

    protected EHS_Presentation presentation;

    public string objectType { protected set; get; }
    public string presentationType { private set; get; }

    public Vector3 position = Vector3.zero;
    public Vector3 scale = Vector3.one;
    public Quaternion rotation = Quaternion.identity;
    public string fgrColor;
    public string bgrColor;

    private string parentID;
    private string objectID;

    protected EHS_AnimationFactory animationFactory;
    protected EHS_ObjectCreateAnimation objectCreateAnimation;
    protected EHS_ObjectDestroyAnimation objectDestroyAnimation;
    protected EHS_ObjectUpdateAnimation objectUpdateAnimation;

    private EHS_Animation runningAnimation;
    private List<EHS_Animation> animations = new List<EHS_Animation>();
    private bool isStopped = true;
    private bool isFirstUpdate = true;

    public EHS_PresentationObject(string id, string type, string objectType, EHS_Presentation presentation){
        this.parentID = id;
        this.presentationType = type;
        this.objectType = objectType;
        this.objectID = id+"_"+type;
        this.presentation = presentation;
        this.animationFactory = new EHS_AnimationFactory(presentation, objectID, objectType);
    }

    public string GetObjectID(){
        return objectID;    
    }

    public string GetParentID(){
        return parentID;    
    }

    public EHS_PresentationConfig GetConfig(){ 
        return presentation.GetConfig();    
    }

    private class ObjectUpdateListener : IObjectUpdateListener {

        private EHS_PresentationObject presentationObject;

        public ObjectUpdateListener(EHS_PresentationObject presentationObject){   
            this.presentationObject = presentationObject;
        }
            
        public void OnUpdated(string type, EHS_Object ehsObject){
            var runningAnimation = presentationObject.runningAnimation;
            if(runningAnimation != null){ 
                runningAnimation.OnObjectUpdated(type, ehsObject);    
            }
        }

        public void OnUpdateFail(string type, EHS_Object ehsObject){
                
        }
    }

    public void InitObject(){ 
        presentation.GetSceneManager().SetObjectCallBack(GetObjectID(), new ObjectUpdateListener(this));
    }

    protected abstract void OnCreate(EHS_PresentationConfig config);

    protected abstract void OnStop();

    protected abstract void OnDestroy();

    protected virtual void ReadFlags(EHS_PresentationConfig config){

    }

    protected virtual EHS_ObjectCreateAnimation DefaultObjectCreateAnimation(){ 
        return null;
    }

    protected virtual EHS_ObjectUpdateAnimation DefaultObjectUpdateAnimation(){ 
        return null;
    }

    protected virtual EHS_ObjectDestroyAnimation DefaultObjectDestroyAnimation(){ 
        return null;
    }

    protected virtual void OnStartUpdate(EHS_PresentationConfig config){ 
        // do nothing if not overriden
    }

    protected virtual void OnEndUpdate(EHS_PresentationConfig config){
        // do nothing if not overriden    
    }

    private void ReplaceAnimation(EHS_PresentationConfig config){
        try{
            if(config.updateAnimationConfig != null && config.updateAnimationConfig.update){
                var animationConfig = config.updateAnimationConfig;
                if(!objectUpdateAnimation.AnimationType().Equals(animationConfig.type)){ 
                    var animation = animationFactory.CreateObjectUpdateAnimation(animationConfig);
                    if(animation != null){ 
                        objectUpdateAnimation = animation;
                        objectUpdateAnimation.OnAnimationReplace(animationConfig);
                    }
                }    
            }
        }catch(Exception e){ 
            Debug.LogException(e);    
        }
    }

    private void CreateTexture(){ 
        var command = EHS_ObjectUtils.CreateObjectCommand(GetParentID(), GetObjectID(), objectType, position, scale, rotation);
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(EHS_DetailType.Clear)
                            .appendConfig(EHS_Details.Side, EHS_QuadObjectSurfaceHandler.AllSides)
                            .appendConfig(EHS_Details.Object, "texture"));
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(EHS_DetailType.View)
                            .appendConfig(EHS_Details.Side, EHS_QuadObjectSurfaceHandler.AllSides)
                            .appendConfig(EHS_Details.BgrColor, bgrColor)
                            .appendConfig(EHS_Details.FgrColor, fgrColor));
        AddCommand(command);
    }

    private void DestroyTexture(){ 
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(objectType);
        command.objectConfig.id = GetObjectID();
        command.objectConfig.keep = true;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                                .setConfigID(EHS_DetailType.Clear)
                                .appendConfig(EHS_Details.Side, EHS_QuadObjectSurfaceHandler.AllSides)
                                .appendConfig(EHS_Details.Object,"texture"));
        AddCommand(command);    
    }

    public void Create(EHS_PresentationConfig config){
        // configure attributes
        fgrColor = config.fgrColor;
        bgrColor = config.bgrColor;
        // create object animations
        objectCreateAnimation = animationFactory.CreateObjectCreateAnimation(config.createAnimationConfig);
        objectUpdateAnimation = animationFactory.CreateObjectUpdateAnimation(config.updateAnimationConfig);
        objectDestroyAnimation = animationFactory.CreateObjectDestroyAnimation(config.destoryAnimationConfig);
        if(objectCreateAnimation == null){ 
            objectCreateAnimation = DefaultObjectCreateAnimation();
        }
        if(objectUpdateAnimation == null){ 
            objectUpdateAnimation = DefaultObjectUpdateAnimation();
        }
        if(objectDestroyAnimation == null){ 
            objectDestroyAnimation = DefaultObjectDestroyAnimation();
        }
        // keep this order for initalisation
        if(objectDestroyAnimation != null){ 
            animations.Add(objectDestroyAnimation);
            objectDestroyAnimation.AddCompleteListener(new DestroyObjectAnimationCompleteListener(this));
        }
        if(objectUpdateAnimation != null){ 
            animations.Add(objectUpdateAnimation);
            objectUpdateAnimation.AddCompleteListener(new UpdateObjectAnimationCompleteListener(this));
        }
        // initialise as last
        if(objectCreateAnimation != null){ 
            animations.Add(objectCreateAnimation);
            objectCreateAnimation.AddCompleteListener(new CreateObjectAnimationCompleteListener(this));
        }
        // read all flags
        ReadFlags(config);
        // setup all attributes and other things needed by config
        OnCreate(config);
        // initialise animations
        foreach(var animation in animations){ 
            animation.InitAnimation(this);
        }
        // create object
        if(!EHS_ObjectType.Empty.Equals(objectType)){
            if(objectType != null){
                CreateTexture();
            }else{ 
                Debug.LogWarning("Failed to create presentation (type:"+presentationType+",id:"
                                   +parentID+") as its object type was not specified.");
            }
        }
    }

    public void Update(EHS_PresentationConfig config){
        OnStartUpdate(config);
        if(isFirstUpdate || isStopped){
            runningAnimation = objectCreateAnimation;
            if(objectCreateAnimation != null){
                presentation.Lock(EHS_Presentation.CreateLock);
                objectCreateAnimation.Animate();   
            }
            if(objectUpdateAnimation != null){
                objectUpdateAnimation.Update(config);
                objectUpdateAnimation.PreAnimate();
            }
            if(!isFirstUpdate && isStopped){
                if(objectUpdateAnimation != null){
                    objectUpdateAnimation.PostStop();
                }   
            }
            isStopped = false; 
            isFirstUpdate = false;
        }else{
            // replace animation if requested
            ReplaceAnimation(config);
            // updated running animation and run it
            runningAnimation = objectUpdateAnimation;
            if(objectUpdateAnimation != null){
                objectUpdateAnimation.Update(config);
                objectUpdateAnimation.Animate();
            }
        }
        OnEndUpdate(config);    
    }

    public void Stop(){
        runningAnimation = objectDestroyAnimation;
        if(objectDestroyAnimation != null){
            presentation.Lock();
            objectDestroyAnimation.Animate();
        }else{
            OnStop();
            if(objectUpdateAnimation != null){
                objectUpdateAnimation.Stop();
            }
            StopMedia(GetParentID());
            StopMedia(GetObjectID());
            DestroyTexture();
            isStopped = true;
        }
    }

    public void Destroy(){ 
        OnDestroy();
    }

    public bool IsStopped(){
        return isStopped;    
    }

    public void SetDestroyAnimationCompletedListener(EHS_Animation.ICompleteListener listener){
        destroyAnimationCompletedListener = listener;
    }

    public void StopMedia(string id){
        try{
            var sceneManager = presentation.GetSceneManager();
            var ehsObject = sceneManager.GetObject(id);
            if(ehsObject != null){
                var mediaSource = ehsObject.GetMediaSourceHandler().GetMediaSource(EHS_MediaSourceType.Video);
                if(mediaSource is EHS_VideoSource){
                    var video = (EHS_VideoSource)mediaSource;
                    if(video != null){
                        video.StopAll();       
                    }
                }
            }
        }catch(Exception e){
            Debug.LogException(e);    
        }
    }

    public void AddCommand(string command){ 
        presentation.AddCommand(command);    
    }

    public void AddCommand(EHS_CommandConfig command){ 
        presentation.AddCommand(command);    
    }

    protected EHS_CommandConfig CreateUpdateCommand(string type){ 
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(type);
        command.objectConfig.id = GetObjectID();
        command.objectConfig.keep = true;    
        return command;
    }

    private class CreateObjectAnimationCompleteListener : EHS_Animation.ICompleteListener {

        private EHS_PresentationObject presentationObject;

        public CreateObjectAnimationCompleteListener(EHS_PresentationObject presentationObject){ 
            this.presentationObject = presentationObject;    
        }
            
        public void OnComplete(){
            presentationObject.presentation.Unlock(EHS_Presentation.CreateLock);
        }
    }

    private class DestroyObjectAnimationCompleteListener : EHS_Animation.ICompleteListener {

        private EHS_PresentationObject presentationObject;

        public DestroyObjectAnimationCompleteListener(EHS_PresentationObject presentationObject){ 
            this.presentationObject = presentationObject;    
        }
            
        public void OnComplete(){
            if(presentationObject.destroyAnimationCompletedListener != null){
                presentationObject.destroyAnimationCompletedListener.OnComplete();
            }
            presentationObject.OnStop();
            if(presentationObject.objectUpdateAnimation != null){
                presentationObject.objectUpdateAnimation.Stop();
            }
            presentationObject.StopMedia(presentationObject.GetParentID());
            presentationObject.StopMedia(presentationObject.GetObjectID());
            presentationObject.DestroyTexture();
            presentationObject.isStopped = true;
            presentationObject.presentation.Unlock();
        }
    }

    private class UpdateObjectAnimationCompleteListener : EHS_Animation.ICompleteListener {

        private EHS_PresentationObject presentationObject;

        public UpdateObjectAnimationCompleteListener(EHS_PresentationObject presentationObject){ 
            this.presentationObject = presentationObject;    
        }
            
        public void OnComplete(){
            // do nothing
        }
    }
}
