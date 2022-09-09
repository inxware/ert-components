
using UnityEngine;

public class EHS_Scale : EHS_Behaviour
{
    public static readonly string TypeName = "scale";

    private bool isScaling = false;
    private Vector3 scale;
    private Vector3 target;
    private Vector3 targetError;
    private float direction = 1f;

    protected override void OnModified()
    {
        target = config.direction;
        direction = (target.x >= transform.localScale.x) ? 1f : -1f;
        targetError = (direction > 0f) ? target - (Vector3.one*config.targetError) : target + Vector3.one*config.targetError;
        scale = (direction > 0f) ? target : transform.localScale; 
        scale.Normalize();
        scale *= direction/100f;
        isScaling = true;
    }

    override protected void Exec()
    {
        if(isScaling){
            if((direction > 0f && targetError.x > transform.localScale.x) || (direction < 0f && targetError.x < transform.localScale.x)){
                transform.localScale += (config.speed * scale);
            }else{
                isScaling = false;
                transform.localScale = target;
                CallBehaviourCompleted();
            }
        }
    }

    public override string Type() {
        return TypeName;
    }
}
