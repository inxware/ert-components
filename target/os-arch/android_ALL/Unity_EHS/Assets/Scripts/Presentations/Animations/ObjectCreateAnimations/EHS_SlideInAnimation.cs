using UnityEngine;

public class EHS_SlideInAnimation : EHS_ObjectCreateAnimation
{
    public static readonly string Name = "slide";

    protected float steps = 1f;
    protected float speed = 9f;
    protected float targetError = 0.02f;
    protected string directionString = EHS_AnimationUtils.Top2Bottom;
    protected Vector3 destination;

    public EHS_SlideInAnimation(EHS_Presentation presentation, string id, string objectTyp) : base(presentation, id, objectTyp){ 
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
        if(presentationObject != null){
            destination = presentationObject.position;
            var direction = EHS_AnimationUtils.Direction(directionString) * -1; // invert
            var objectBounds = presentation.GetConfig().zoneConfig.ToRect();
            presentationObject.position += EHS_AnimationUtils.OutOfScreenPosition(direction, objectBounds);
        }
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
        ehsObject.SetBehaviourCallBack(eventType, new SlideInBehaviourListener(this));
        command.eventConfig.notifyOnComplete = true;
        AddCommand(command);
    }

    private class SlideInBehaviourListener : IBehaviourListener {

        private EHS_SlideInAnimation animationObject;

        public SlideInBehaviourListener(EHS_SlideInAnimation animationObject){
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
