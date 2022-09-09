
using UnityEngine;

public class EHS_FadeOutAnimation : EHS_ObjectDestroyAnimation
{
    public static readonly string Name = "fadeOut";
    public static readonly string FadeOut = EHS_DetailType.Fade + "_out";

    public string fgrColor;
    public string bgrColor;
    protected float fadeSpeed = 1f;

    public EHS_FadeOutAnimation(EHS_Presentation presentation, string id, string objectTyp) : base(presentation, id, objectTyp){ 
    }

    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        if(presentationObject != null){
            fgrColor = presentationObject.fgrColor;
            bgrColor = presentationObject.bgrColor;
        }
        if(animationConfig != null){ 
            fadeSpeed = (animationConfig.speed > 0f) ? animationConfig.speed : fadeSpeed;
        }
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        if(!string.IsNullOrEmpty(type) && type.Equals(FadeOut)){ 
            CompleteAnimation();
        }
    }

    public override void Animate() {
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(objectType);
        command.objectConfig.id = id;
        command.objectConfig.keep = true;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(EHS_DetailType.Fade)
                            .appendConfig(EHS_Details.Side, EHS_QuadObjectSurfaceHandler.AllSides)
                            .appendConfig(EHS_Details.BgrColor, bgrColor)
                            .appendConfig(EHS_Details.FgrColor, fgrColor)
                            .appendConfig("direction", "out")
                            .appendConfig("fadeSpeed", fadeSpeed));
        AddCommand(command);
    }
}
