
using UnityEngine;

public class EHS_BoundsConfig : EHS_Config
{
    public static readonly string TypeName = "bounds";

    public float top = 0f;
    public float left = 0f;
    public float bottom = 1f;
    public float right = 1f;

    public EHS_BoundsConfig() : base(TypeName)
    {
    }

    public EHS_BoundsConfig(Rect rect) : base(TypeName)
    {
        top = rect.x;
        left = rect.y;
        bottom = rect.height;
        right = rect.width;
    }

    public Rect ToRect(){ 
        return new Rect(top, left, right, bottom);    
    }
}
