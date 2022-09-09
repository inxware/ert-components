using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_Scroll : EHS_Behaviour
{
    public static readonly string TypeName = "scroll";

    private bool isScrolling = false;

    protected override void OnModified()
    {
        isScrolling = true;
    }

    protected override void OnRemove()
    {
        isScrolling = false;
    }

    protected override void Exec()
    {
        if (isScrolling){
            
        }
    }

    public override string Type() {
        return TypeName;
    }
}
