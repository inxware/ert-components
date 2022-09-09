
using UnityEngine;

public class EHS_DeflateAnimation : EHS_ObjectDestroyAnimation
{
    public static readonly string Name = "deflate";

    public float targetScale = 0f;

    protected float steps = 0.05f;
    protected float speed = 5f;
    protected float targetError = 0.02f;
    protected Vector3 destination;

    public EHS_DeflateAnimation(EHS_Presentation presentation, string id, string objectTyp) : base(presentation, id, objectTyp){ 
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
            targetScale = (1f > targetScale) ? targetScale : 1f;
            destination = presentationObject.scale * targetScale;
        }else{ 
            destination = Vector3.zero;
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
        command.eventConfig.direction = destination;
        command.eventConfig.targetError = targetError;
        ehsObject.SetBehaviourCallBack(eventType, new DeflateBehaviourListener(this));
        command.eventConfig.notifyOnComplete = true;
        AddCommand(command);
    }

    private class DeflateBehaviourListener : IBehaviourListener {

        private EHS_DeflateAnimation animationObject;

        public DeflateBehaviourListener(EHS_DeflateAnimation animationObject){
            this.animationObject = animationObject;
        }
        
        public void OnCompleted(){
            if(animationObject.presentationObject != null){
                animationObject.presentationObject.scale = animationObject.destination;
            }
            animationObject.CompleteAnimation();
        }
    }
}
