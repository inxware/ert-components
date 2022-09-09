
using UnityEngine;

public class EHS_InflateAnimation : EHS_ObjectCreateAnimation
{
    public static readonly string Name = "inflate";

    protected float steps = 0.05f;
    protected float speed = 5f;
    protected float targetError = 0.01f;
    protected Vector3 scale;

    public EHS_InflateAnimation(EHS_Presentation presentation, string id, string objectTyp) : base(presentation, id, objectTyp){ 
    }

    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        if(animationConfig != null){ 
            steps = (animationConfig.steps > 0f) ? animationConfig.steps : steps;
            speed = (animationConfig.speed > 0f) ? animationConfig.speed : speed;
        }
        if(presentationObject != null){
            scale = presentationObject.scale;
            presentationObject.scale = Vector3.zero;
        }
    }

    public override void Animate() {
        var ehsObject = presentation.GetSceneManager().GetObject(id);
        var command = new EHS_CommandConfig("attachEvent");
        var eventType = EHS_Scale.TypeName;
        command.eventConfig = new EHS_EventConfig(eventType);
        command.eventConfig.id = id;
        command.eventConfig.speed = speed;
        command.eventConfig.steps = steps;
        command.eventConfig.direction = scale;
        command.eventConfig.targetError = targetError;
        ehsObject.SetBehaviourCallBack(eventType, new InflateBehaviourListener(this));
        command.eventConfig.notifyOnComplete = true;
        AddCommand(command);
    }

    private class InflateBehaviourListener : IBehaviourListener {

        private EHS_InflateAnimation animationObject;

        public InflateBehaviourListener(EHS_InflateAnimation animationObject){
            this.animationObject = animationObject;
        }
        
        public void OnCompleted(){
            if(animationObject.presentationObject != null){
                animationObject.presentationObject.scale = animationObject.scale;
            }
            animationObject.CompleteAnimation();
        }
    }
}
