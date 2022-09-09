
using UnityEngine;

public class EHS_MoveWithin : EHS_MoveTo
{
    public new static readonly string TypeName = "movewithin";

    protected override void OnModified(){

        base.OnModified();
    }

    protected override bool CallBehaviourCompleted(){
        if(config.freeze.x == 0f){
            config.direction.x *= -1;
        }
        if(config.freeze.y == 0f){
            config.direction.y *= -1;
        }
        if(config.freeze.z == 0f){
            config.direction.z *= -1;
        }
        Modify(config);
        return false;
    }
}
