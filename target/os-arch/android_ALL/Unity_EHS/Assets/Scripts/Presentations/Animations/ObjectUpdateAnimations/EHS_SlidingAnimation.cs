using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_SlidingAnimation : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "slide";

    private EHS_SlideOutAnimation slideOut;
    private EHS_SlideInAnimation slideIn;
    private EHS_AnimationConfig animationConfig;
    private Vector3 originalPosition;

    public EHS_SlidingAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType) { 
        
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
                originalPosition = presentationObject.position;
                slideOut = new EHS_SlideOutAnimation(presentation, id, objectType);
                slideOut.SetPresentationObject(presentationObject);
                slideOut.InitAnimation(animationConfig);
                slideOut.AddCompleteListener(new SlideOutCompleteListener(this));
                presentation.Lock();
                slideOut.Animate();
            }
        }
    }

    public override void Stop(){
        ClearText(initalSide);
    }

    private class SlideOutCompleteListener : ICompleteListener {

        private EHS_SlidingAnimation animation;

        public SlideOutCompleteListener(EHS_SlidingAnimation animation){ 
            this.animation = animation;
            animation.ClearText(animation.initalSide);
        }
            
        public void OnComplete(){
            animation.ClearTexture(animation.initalSide);
            var presentation = animation.presentation;
            animation.slideIn = new EHS_SlideInAnimation(presentation, animation.id, animation.objectType);
            animation.presentationObject.position = animation.originalPosition;
            animation.slideIn.SetPresentationObject(animation.presentationObject);
            animation.slideIn.InitAnimation(animation.animationConfig);
            UpdatePosition(animation.presentationObject.position);
            animation.slideIn.AddCompleteListener(new SlideInCompleteListener(animation));
            if(animation.IsApplyingMedia()){
                animation.AddMedia(animation.config, animation.initalSide);
            }
            animation.slideIn.Animate();
        }

        private void UpdatePosition(Vector3 position){ 
            EHS_CommandConfig command = new EHS_CommandConfig("updateObject");
            // create object config
            command.objectConfig = new EHS_ObjectConfig(animation.objectType);
            command.objectConfig.id = animation.id;
            command.objectConfig.position = position;
            command.objectConfig.rotation = animation.presentationObject.rotation;
            command.objectConfig.scale = animation.presentationObject.scale;
            animation.AddCommand(command);   
        }
    }

    private class SlideInCompleteListener : ICompleteListener {

        private EHS_SlidingAnimation animation;

        public SlideInCompleteListener(EHS_SlidingAnimation animation){ 
            this.animation = animation;    
        }
            
        public void OnComplete(){
            var presentation = animation.presentation;
            if(animation.IsTextAvailable()){
                animation.AddText(animation.config, animation.initalSide);
            }
            presentation.Unlock();
        }
    }
}