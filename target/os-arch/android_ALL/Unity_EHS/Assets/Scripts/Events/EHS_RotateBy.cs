
using UnityEngine;

public class EHS_RotateBy : EHS_Behaviour
{
    public static readonly string TypeName = "rotateby";

    private bool isRotating = false;
    private Vector3 original;
    private Vector3 target;
    private float targetError = 0.01f;

    protected override void OnModified()
    {
        original = transform.eulerAngles;
        target = config.direction;
        targetError = (config.targetError > 0f) ? config.targetError : targetError;
        isRotating = true;
    }

    protected override void OnRemove()
    {
        isRotating = false;
        transform.eulerAngles = original;
    }

    protected override void Exec()
    {
        if (isRotating){
            float distance = Vector3.Distance(transform.eulerAngles, target);
            //Debug.Log("Distance: "+distance+", eulerAngles:"+transform.eulerAngles+",traget:"+target);
            if (distance > targetError){
                transform.eulerAngles = Vector3.Lerp(transform.rotation.eulerAngles, target, Time.deltaTime * config.speed);
            }else{
                transform.eulerAngles = target;
                isRotating = false;
                config = null;
                CallBehaviourCompleted();
            }
        }
    }

    public override string Type() {
        return TypeName;
    }
}
