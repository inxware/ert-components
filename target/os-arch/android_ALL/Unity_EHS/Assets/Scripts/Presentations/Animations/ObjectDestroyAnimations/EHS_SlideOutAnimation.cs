using UnityEngine;

public class EHS_SlideOutAnimation : EHS_ObjectDestroyAnimation
{
    public static readonly string Name = "slide";

    protected float steps = 1f;
    protected float speed = 9f;
    protected float targetError = 0.05f;
    protected string directionString = EHS_AnimationUtils.Top2Bottom;
    protected Vector3 destination;

    public EHS_SlideOutAnimation(EHS_Presentation presentation, string id, string objectTyp) : base(presentation, id, objectTyp){ 
    }

    public override string AnimationType() {
        return Name;
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        if(animationConfig != null){ 
            steps = (animationConfig.steps > 0f) ? animationConfig.steps : steps;
            speed = (animationConfig.speed > 0f) ? animationConfig.speed : speed;
            directionString = EHS_AnimationUtils.IsDirection(animationConfig.direction) ? animationConfig.direction : directionString;
        }
        var direction = EHS_AnimationUtils.Direction(directionString);
        var objectBounds = presentation.GetConfig().zoneConfig.ToRect();
        destination = EHS_AnimationUtils.OutOfScreenPosition(direction, objectBounds);
    }

    public override void Animate() {
        var ehsObject = presentation.GetSceneManager().GetObject(id);
        var command = new EHS_CommandConfig("attachEvent");
        var eventType = EHS_MoveTo.TypeName; 
        command.eventConfig = new EHS_EventConfig(eventType);
        command.eventConfig.id = id;
        command.eventConfig.speed = speed;
        command.eventConfig.steps = steps;
        command.eventConfig.direction = destination;
        command.eventConfig.targetError = targetError;
        ehsObject.SetBehaviourCallBack(eventType, new SlideOutBehaviourListener(this));
        command.eventConfig.notifyOnComplete = true;
        AddCommand(command);
    }

    private class SlideOutBehaviourListener : IBehaviourListener {

        private EHS_SlideOutAnimation animationObject;

        public SlideOutBehaviourListener(EHS_SlideOutAnimation animationObject){
            this.animationObject = animationObject;
        }
        
        public void OnCompleted(){
            if(animationObject.presentationObject != null){
                animationObject.presentationObject.position = animationObject.destination;
            }
            animationObject.CompleteAnimation();
        }
    }
}
