using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_Move : EHS_Behaviour
{
    public static readonly string TypeName = "move";

    override protected void Exec()
    {
        // Move the object forward along its z axis 1 unit/second.
        transform.Translate(config.direction * Time.deltaTime * config.speed);
    }

    public override string Type() {
        return TypeName;
    }
}
