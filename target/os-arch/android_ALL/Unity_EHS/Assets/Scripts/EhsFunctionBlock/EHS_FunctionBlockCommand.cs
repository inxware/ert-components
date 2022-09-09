using UnityEngine;

public class EHS_FunctionBlockCommand : ICommand
{
    public static readonly string CREATE_COMMAND        = "create";
    public static readonly string SET_MEDIA_COMMAND     = "setmedia";
    public static readonly string STOP_MEDIA_COMMAND    = "stop";
    public static readonly string DESTROY_COMMAND       = "destroy";

    public static readonly string TypeName = "ehsFunctionBlock";

    protected EHS_FunctionBlockManager fbmanager;
    protected EHS_SceneManager smanager;
    private EHS_FunctionBlockConfig config;

    public EHS_FunctionBlockCommand(EHS_FunctionBlockManager fbmanager, EHS_SceneManager smanager,
                                    EHS_FunctionBlockConfig config)
    {
        this.fbmanager = fbmanager;
        this.smanager = smanager;
        this.config = config;
        if(this.config.parameters != null){
            // should always be presentation if not specified.
            if(string.IsNullOrEmpty(this.config.type)){
                this.config.type = EHS_ObjectType.Presentation;
            }
            // select correct media type
            if(config.cmd.Equals(SET_MEDIA_COMMAND)){ 
                config.type = EHS_MediaSource.SelectMedia(config.type, config.path);
            }
            // @TODO - make depth the same as width for now
            this.config.parameters.d = this.config.parameters.w;
        }
    }

    public string Type()
    {
        return TypeName;
    }

    public bool Exec()
    {
        if(config.cmd == CREATE_COMMAND){
            return fbmanager.CreateObject(config) != null;
        }else if(config.cmd == DESTROY_COMMAND){
            fbmanager.DestroyObject(config.id);
            return true;
        }else{
            EHS_FBObject ehsFbObject = fbmanager.GetObject(config.id);
            if(ehsFbObject != null){
                if(config.cmd == SET_MEDIA_COMMAND){
                    return fbmanager.SetMedia(config);
                }else if(config.cmd == STOP_MEDIA_COMMAND){
                    return fbmanager.StopMedia(config);
                }else{
                    Debug.Log("Invalid EHS function block command (" + config.cmd + ").");
                }
            }
        }
        return false;
    }
}
