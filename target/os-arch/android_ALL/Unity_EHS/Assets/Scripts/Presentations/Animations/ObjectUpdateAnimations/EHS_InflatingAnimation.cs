using System.Collections.Generic;
using UnityEngine;

public class EHS_InflatingAnimation  : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "inflate";

    private EHS_DeflateAnimation deflate;
    private EHS_InflateAnimation inflate;
    private EHS_AnimationConfig animationConfig;
    private Vector3 originalScale;

    public EHS_InflatingAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType) { 
        
    }
    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        this.animationConfig = animationConfig;
        var objectMedia = new ObjectMedia(initalSide);
        objectMediaMap = new Dictionary<string, ObjectMedia>();
        objectMediaMap.Add(initalSide, objectMedia);
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        // do nothing
    }

    public override void Animate(){
        if(IsTextAvailable()){
            AddText(config, initalSide);
        }else{
            if(IsApplyingMedia()){
                originalScale = presentationObject.scale;
                deflate = new EHS_DeflateAnimation(presentation, id, objectType);
                deflate.targetScale = 0.05f;
                deflate.SetPresentationObject(presentationObject);
                deflate.InitAnimation(animationConfig);
                deflate.AddCompleteListener(new DeflateCompleteListener(this));
                presentation.Lock();
                deflate.Animate();
            }
        }
    }

    public override void Stop(){
        ClearText(initalSide);
    }

    private class DeflateCompleteListener : ICompleteListener {

        private EHS_InflatingAnimation animation;

        public DeflateCompleteListener(EHS_InflatingAnimation animation){ 
            this.animation = animation;
            animation.ClearText(animation.initalSide);
        }
            
        public void OnComplete(){
            animation.ClearTexture(animation.initalSide);
            var presentation = animation.presentation;
            animation.inflate = new EHS_InflateAnimation(presentation, animation.id, animation.objectType);
            animation.presentationObject.scale = animation.originalScale;
            animation.inflate.SetPresentationObject(animation.presentationObject);
            animation.inflate.InitAnimation(animation.animationConfig);
            animation.inflate.AddCompleteListener(new InflateCompleteListener(animation));
            if(animation.IsApplyingMedia()){
                animation.AddMedia(animation.config, animation.initalSide);
            }
            if(animation.IsTextAvailable()){
                animation.AddText(animation.config, animation.initalSide);
            }
            animation.inflate.Animate();
        }
    }

    private class InflateCompleteListener : ICompleteListener {

        private EHS_InflatingAnimation animation;

        public InflateCompleteListener(EHS_InflatingAnimation animation){ 
            this.animation = animation;    
        }
            
        public void OnComplete(){
            var presentation = animation.presentation;

            presentation.Unlock();
        }
    }
}
