
using UnityEngine;

public class EHS_BackgroundPresentation : EHS_Presentation
{
    private string background_ID;

    private EHS_QuadPresentation.QuadPresentationObject quadObject;

    private EHS_PresentationConfig quadConfig;
    private EHS_PresentationConfig fadingConfig;

    private float fadeSteps = 0.1f;
    private float fadeDelay = 0.05f;

    private string lastMedia;
    
    private enum StoppedMedia{ 
        NONE,
        IMAGE,
        VIDEO,
    };
    StoppedMedia stoppedMedia;

    protected override void OnCreate()
    {
        var config = GetConfig();
        string parentId = config.id;
        background_ID = parentId  + "_" + EHS_PresentationType.Background;

        EHS_CommandConfig command = new EHS_CommandConfig("createObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.parentID = parentId;
        command.objectConfig.id = background_ID;
        string bgrColor = config.bgrColor;
        bgrColor = EHS_Utils.ChangeAlpha(bgrColor, 0f);
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_DetailType.Color)
                            .appendConfig(EHS_Details.BgrColor, bgrColor));

        fadeSteps = 1f/(float)config.fadeConfig.steps;
        fadeDelay = config.fadeConfig.delay;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_DetailType.Fade)
                            .appendConfig("direction", "in")
                            .appendConfig("fadeSteps", fadeSteps)
                            .appendConfig("fadeDelay", fadeDelay));
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_DetailType.Fade)
                            .appendConfig("direction", "out")
                            .appendConfig("fadeSteps", fadeSteps)
                            .appendConfig("fadeDelay", fadeDelay));
        AddCommand(command);
        quadObject = new EHS_QuadPresentation.QuadPresentationObject(parentId, this);
        var view = EHS_Utils.View();
        quadConfig = GetConfig().Copy();
        quadConfig.bgrColor = "0.0.0.0";
        float offset = 1f;
        float distance = Mathf.Abs(view.transform.position.z);
        float increase = (offset + distance)/distance;
        quadObject.position.z = offset;
        quadConfig.zoneConfig.right*=increase;
        quadConfig.zoneConfig.bottom*=increase;
        quadConfig.createAnimationConfig = new EHS_AnimationConfig(EHS_FadeInAnimation.Name);
        quadConfig.createAnimationConfig.speed = 0.5f;
        quadConfig.destoryAnimationConfig = new EHS_AnimationConfig(EHS_FadeOutAnimation.Name);
        quadConfig.destoryAnimationConfig.speed = 0.5f;
        quadConfig.updateAnimationConfig = new EHS_AnimationConfig(EHS_FadingAnimation.Name);
        quadConfig.updateAnimationConfig.speed = 0.5f;
        quadObject.Create(quadConfig);
        InitQuad();
        stoppedMedia = StoppedMedia.NONE;
    }

    public override void StopMedia()
    {
        quadObject.ClearText();
        if(IsPreviousVideo()){
            fadingConfig = null;
            StopVideo();
            stoppedMedia = StoppedMedia.VIDEO;
        }else{
            quadObject.Stop();
            stoppedMedia = StoppedMedia.IMAGE;
        }
    }

    public override void OnPresentationDestroy(EHS_Object ehsObject)
    {
        try{
            new EHS_ViewHandler.ViewDetailHandler(manager).Stop(ehsObject);
        }catch(System.Exception e){ 
            Debug.LogException(e);
        }
    }

    protected override void InitExec(){ 
        quadObject.InitObject();
        GetSceneManager().SetObjectCallBack(background_ID, new ObjectStopListener(this));
    }

    protected override void Exec(EHS_PresentationConfig config)
    {
        string type = config.type;
        string path = config.path;
        string text = config.text;
        
        if(type.Equals(EHS_MediaSourceType.Video)){
            if(IsPreviousVideo() || stoppedMedia == StoppedMedia.IMAGE){
                InitQuad(); // init empty quad for text
                SetVideo(path);
            }else{ 
                // stop previous media and set new media from 
                // its fade callback
                quadObject.SetDestroyAnimationCompletedListener(new QuadFadingListener(this));
                fadingConfig = config.Copy();
                quadObject.Stop();
                // locking is done by quad handler
            }
            lastMedia = type;
        }else if(type.Equals(EHS_MediaSourceType.Image)){
            if(IsPreviousImage() || stoppedMedia == StoppedMedia.VIDEO){
                SetQuad(type, path, text);
            }else{ 
                // stop previous media and set new media from 
                // its fade callback
                fadingConfig = config.Copy();
                StopVideo();
            }
            lastMedia = type;
        }else if(type.Equals("text")){ 
            SetQuad(type, path, text);
        }else{ 
            Debug.Log("Unknown background presentation type (" + type + ")");
            return;
        }
        if(stoppedMedia != StoppedMedia.NONE){
            stoppedMedia = StoppedMedia.NONE;   
        }
    }

    private void StopVideo(){
        EHS_CommandConfig command = new EHS_CommandConfig("stopObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.id = background_ID;
        AddCommand(command);
        Lock();
    }

    private bool IsPreviousVideo(){
        return string.IsNullOrEmpty(lastMedia) || lastMedia.Equals(EHS_MediaSourceType.Video);    
    }

    private bool IsPreviousImage(){
        return string.IsNullOrEmpty(lastMedia) || lastMedia.Equals(EHS_MediaSourceType.Image);    
    }

    private void SetVideo(string path){
        EHS_CommandConfig command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.keep = true;
        command.objectConfig.id = background_ID;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.VideoControl)
                            .appendConfig(EHS_Details.Path, path));
        AddCommand(command);
    }

    private void InitQuad(){ 
        SetQuad("text", "", "");    
    }

    private void SetQuad(string type, string path, string text){
        quadConfig.text = text;
        quadConfig.type = type;
        quadConfig.path = path;
        quadObject.Update(quadConfig);
    }

    private void PostFadeOut(){
        quadObject.SetDestroyAnimationCompletedListener(null);
        if(fadingConfig == null){ 
            return;    
        }
        if(fadingConfig.type.Equals(EHS_MediaSourceType.Video)){
            SetVideo(fadingConfig.path);
        }else if(fadingConfig.type.Equals(EHS_MediaSourceType.Image)){
            SetQuad(fadingConfig.type, fadingConfig.path, fadingConfig.text);
        }
    }

    private class ObjectStopListener : IObjectStopListener 
    {
        private EHS_BackgroundPresentation presentation;
        public ObjectStopListener(EHS_BackgroundPresentation presentation){ 
            this.presentation = presentation;    
        }
        public void OnStop(EHS_Object ehsObject, string type, bool success){
            if(type.Equals(EHS_ViewHandler.ViewDetailHandler.STOP_ID)){
                // unlock presentation
                presentation.PostFadeOut();
                presentation.Unlock();
                if(presentation.fadingConfig == null){
                    presentation.InitQuad();
                }
            }
        }
    }

    private class QuadFadingListener : EHS_Animation.ICompleteListener
    { 
        private EHS_BackgroundPresentation presentation;
        public QuadFadingListener(EHS_BackgroundPresentation presentation)
        { 
            this.presentation = presentation;
        }
        public void OnComplete(){ 
            presentation.PostFadeOut();
        }
    }
}
