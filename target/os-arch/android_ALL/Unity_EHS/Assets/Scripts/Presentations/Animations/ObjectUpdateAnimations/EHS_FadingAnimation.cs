using System.Collections.Generic;

public class EHS_FadingAnimation : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "fade";

    private string fgrColor;
    private string bgrColor;
    private EHS_FadeOutAnimation fadeOut;
    private EHS_FadeInAnimation fadeIn;
    private EHS_AnimationConfig animationConfig;

    public EHS_FadingAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType) { 
        
    }
    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        if(presentationObject != null){
            fgrColor = presentationObject.fgrColor;
            bgrColor = presentationObject.bgrColor;
        }
        this.animationConfig = animationConfig;
        var objectMedia = new ObjectMedia(initalSide);
        objectMediaMap = new Dictionary<string, ObjectMedia>();
        objectMediaMap.Add(initalSide, objectMedia);
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        if(!string.IsNullOrEmpty(type)){
            if(type.Equals(EHS_FadeOutAnimation.FadeOut) && (fadeOut != null)){ 
                fadeOut.CompleteAnimation();
                fadeOut = null;
            }else if(type.Equals(EHS_FadeInAnimation.FadeIn) && (fadeIn != null)){ 
                fadeIn.CompleteAnimation();
                fadeIn = null;
            }
        }
    }

    public override void Animate(){
        if(IsTextAvailable()){
            AddText(config, initalSide);
        }else{
            if(IsApplyingMedia()){
                fadeOut = new EHS_FadeOutAnimation(presentation, id, objectType);
                fadeOut.InitAnimation(animationConfig);
                fadeOut.bgrColor = bgrColor;
                fadeOut.fgrColor = fgrColor;
                fadeOut.AddCompleteListener(new FadeOutCompleteListener(this));
                presentation.Lock();
                fadeOut.Animate();
            }
        }
    }

    public override void Stop(){
        ClearText(initalSide);
    }

    private class FadeOutCompleteListener : ICompleteListener {

        private EHS_FadingAnimation animation;

        public FadeOutCompleteListener(EHS_FadingAnimation animation){ 
            this.animation = animation;
            animation.ClearText(animation.initalSide);
        }
            
        public void OnComplete(){
            animation.ClearTexture(animation.initalSide);
            var presentation = animation.presentation;
            animation.fadeIn = new EHS_FadeInAnimation(presentation, animation.id, animation.objectType);
            animation.fadeIn.bgrColor = animation.bgrColor;
            animation.fadeIn.fgrColor = animation.fgrColor;
            animation.fadeIn.InitAnimation(animation.animationConfig);
            animation.fadeIn.AddCompleteListener(new FadeInCompleteListener(animation));
            if(animation.IsApplyingMedia()){
                animation.AddMedia(animation.config, animation.initalSide);
            }
            if(animation.IsTextAvailable()){
                animation.AddText(animation.config, animation.initalSide);
            }
            animation.fadeIn.Animate();
        }
    }

    private class FadeInCompleteListener : ICompleteListener {

        private EHS_FadingAnimation animation;

        public FadeInCompleteListener(EHS_FadingAnimation animation){ 
            this.animation = animation;    
        }
            
        public void OnComplete(){
            var presentation = animation.presentation;

            presentation.Unlock();
        }
    }
}
