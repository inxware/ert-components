

public abstract class EHS_ObjectDestroyAnimation : EHS_Animation
{
    public EHS_ObjectDestroyAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType){ 
        
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        // do nothing
    }

    public override void InitAnimation(EHS_PresentationObject presentationObject){ 
        this.presentationObject = presentationObject;
        EHS_AnimationConfig animationConfig = presentationObject.GetConfig().destoryAnimationConfig;
        InitAnimation(animationConfig);
    }
}
