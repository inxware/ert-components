
using UnityEngine;

public class EHS_BackgroundPresentation : EHS_Presentation
{
    private string background_ID;

    private static readonly float fadeSpeed = 1.0f;

    private bool isAnimating = false;

    protected override void OnCreate()
    {
        var config = GetConfig();
        string parentId = config.id;
        background_ID = parentId  + "_" + EHS_PresentationType.Background;

        isAnimating = (config.updateAnimationConfig != null && !string.IsNullOrEmpty(config.updateAnimationConfig.type) && 
                      !config.updateAnimationConfig.type.Equals(EHS_NoAnimation.Name));

        if(config.textConfig != null){
            config.textConfig.fontScale *= 0.1f;
        }

        EHS_CommandConfig command = new EHS_CommandConfig("createObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.parentID = parentId;
        command.objectConfig.id = background_ID;
        string bgrColor = config.bgrColor;
        bgrColor = (isAnimating) ? EHS_Utils.ChangeAlpha(bgrColor, 0.0f) : EHS_Utils.ChangeAlpha(bgrColor, 1.0f);
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_DetailType.Color)
                            .appendConfig(EHS_Details.BgrColor, bgrColor));
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.DisableAll));
        if(isAnimating){
            command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                                .setConfigID(EHS_DetailType.Fade)
                                .appendConfig("direction", "in")
                                .appendConfig("fadeSpeed", fadeSpeed));
            command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                                .setConfigID(EHS_DetailType.Fade)
                                .appendConfig("direction", "out")
                                .appendConfig("fadeSpeed", fadeSpeed));
        } else {
            command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                                .setConfigID(EHS_DetailType.Fade)
                                .appendConfig("direction", "disable"));
        }
        AddCommand(command);
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
        GetSceneManager().SetObjectCallBack(background_ID, new ObjectStopListener(this));
    }

    protected override void Exec(EHS_PresentationConfig config)
    {
        string type = config.type;
        string path = config.path;
        string text = config.text;

        EHS_CommandConfig command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.keep = true;
        command.objectConfig.id = background_ID;

        ClearText(); // clear text
        
        if(type.Equals(EHS_MediaSourceType.Video)){
            // create media source
            SetVideo(command, path);
        }else if(type.Equals(EHS_MediaSourceType.Image)){
            // create media source
            AddCommand(EHS_MediaSource.CreateCommand(background_ID, type, path));
            SetImage(command, path);
        }else if(type.Equals("text")){ 
            SetText(command, config.textConfig, text);
        }else{ 
            Debug.Log("Unknown background presentation type (" + type + ")");
            return;
        }

        AddCommand(command);
    }

    private void SetVideo(EHS_CommandConfig command, string path){
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.VideoControl)
                            .appendConfig(EHS_Details.Path, path));
    }

    private void SetImage(EHS_CommandConfig command, string path){ 
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.ImageControl)
                            .appendConfig(EHS_Details.Path, path));
    }

    private void SetText(EHS_CommandConfig command, EHS_TextConfig config, string text){ 
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.TextControl)
                            .appendConfig(EHS_Details.Text, text)
                            .appendConfig(config));
    }

    private void ClearText(){ 
        EHS_CommandConfig command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        //command.objectConfig.keep = true;
        command.objectConfig.id = background_ID;

        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.View)
                            .setConfigID(EHS_ViewHandler.TextControl)
                            .appendConfig("Clear", true));

        AddCommand(command);
    }

    public override void StopMedia()
    {
        EHS_CommandConfig command = new EHS_CommandConfig("stopObject");
        command.objectConfig = new EHS_ObjectConfig(EHS_ObjectType.View);
        command.objectConfig.id = background_ID;
        AddCommand(command);
        Lock();
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
                presentation.Unlock();
            }
        }
    }
}
