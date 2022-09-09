using System.Collections.Generic;
using UnityEngine;

public class EHS_ScrollAnimation : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "scroll";

    protected float steps = 0.05f;
    protected float speed = 5f;
    protected float targetError = 0.01f;
    protected Vector3 direction;
    protected string image;
    protected EHS_Object ehsObject;

    public EHS_ScrollAnimation(EHS_Presentation presentation, string id, string objectType) : base(presentation, id, objectType) { 
        
    }

    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        var objectMedia = new ObjectMedia(initalSide);
        objectMediaMap = new Dictionary<string, ObjectMedia>();
        objectMediaMap.Add(initalSide, objectMedia);
        if(animationConfig != null){ 
            steps = (animationConfig.steps > 0f) ? animationConfig.steps : steps;
            speed = (animationConfig.speed > 0f) ? animationConfig.speed : speed;
        }
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        // do nothing
    }

    public override void Stop(){
        ClearText(initalSide);
    }

    public override void PreAnimate() {
        ehsObject = presentation.GetSceneManager().GetObject(id);
    }

    public override void Animate(){
        if(!config.IsTextOnly() && IsApplyingMedia()){
            ClearText(initalSide);
            Scroll(new ScrollBehaviourListener(this));
        }else{ 
            ApplyText();
        }
    }

    protected void ApplyMedia(){ 
        // is non-text media
        if(IsApplyingMedia()){
            AddMedia(config, initalSide);
        }
    }

    protected void ApplyText(){ 
         // is text media
        if(IsTextAvailable()){
            AddText(config, initalSide);
        }
    }

    private void Scroll(IBehaviourListener listener){ 
        var command = new EHS_CommandConfig("attachEvent");
        command.eventConfig = new EHS_EventConfig(EHS_RotateBy.TypeName);
        command.eventConfig.id = id;
        command.eventConfig.speed = speed;
        command.eventConfig.steps = steps;
        command.eventConfig.direction = direction;
        command.eventConfig.targetError = targetError;
        ehsObject.SetBehaviourCallBack(EHS_Scroll.TypeName, listener);
        command.eventConfig.notifyOnComplete = true;
        AddCommand(command);
    }

    private class ScrollBehaviourListener : IBehaviourListener {

        private EHS_ScrollAnimation animationObject;

        public ScrollBehaviourListener(EHS_ScrollAnimation animationObject){
            this.animationObject = animationObject;

        }
        
        public void OnCompleted(){
        }
    }
}
