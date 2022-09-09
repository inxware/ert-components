using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_Rotate : EHS_Behaviour
{
    public static readonly string TypeName = "rotate";

    override protected void Exec()
    {
        transform.Rotate(config.direction, Time.deltaTime * config.speed, Space.Self);
    }

    public override string Type() {
        return TypeName;
    }
}
