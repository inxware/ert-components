
using UnityEngine;

public class EHS_MoveTo : EHS_Behaviour
{
    public static readonly string TypeName = "moveto";

    private bool isMoving = false;
    private Vector3 target;
    private float targetError = 0.01f;

    protected override void OnModified()
    {
        isMoving = true;
        targetError = (config.targetError > 0f) ? config.targetError : targetError;
        target = config.direction;
    }

    override protected void Exec()
    {
        if(isMoving){
            float distance = Vector3.Distance(transform.localPosition, target);
            if(distance > targetError){
                // Move the object forward along its z axis 1 unit/second.
                transform.localPosition = Vector3.MoveTowards(transform.localPosition, target, Time.deltaTime * config.speed);
            }else{
                transform.localPosition = target;
                isMoving = false;
                CallBehaviourCompleted();
            }
        }
    }

    public override string Type() {
        return TypeName;
    }
}
