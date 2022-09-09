using System.Collections.Generic;

public class EHS_NoAnimation : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "noanimation";

    public EHS_NoAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType) { 
        
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        var objectMedia = new ObjectMedia(initalSide);
        objectMediaMap = new Dictionary<string, ObjectMedia>();
        objectMediaMap.Add(initalSide, objectMedia);
    }

    public override string AnimationType() {
        return Name;
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        // do nothing
    }

    protected override bool IsApplyingMedia(){ 
        return !config.IsTextOnly() && !string.IsNullOrEmpty(config.type) && !string.IsNullOrEmpty(config.path);
    }

    public override void Animate(){
        if(IsApplyingMedia()){
            ObjectMedia objectMedia = GetObjectMedia(initalSide);
            if(objectMedia.HasMedia()){ 
                Clear();    
            }
            AddMedia(config, initalSide);
        }
        if(IsTextAvailable()){
            AddText(config, initalSide);
        }
    }

    public override void Stop(){
        ClearText(initalSide);
    }
}
