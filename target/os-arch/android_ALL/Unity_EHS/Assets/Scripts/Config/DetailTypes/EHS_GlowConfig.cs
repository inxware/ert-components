using UnityEngine;

[System.Serializable]
public class EHS_GlowConfig : EHS_Config
{
    public static readonly string TypeName = "glowConfig";

    public bool enabled = false;
    public string color = "15.142.23.123";
    public float offset = 0.25f; // {-1,1}
    public float inner = 0.40f;  // {0,1}
    public float outer = 0.6f;     // {0,1}
    public float power = 0.73f;  // {0,1}

    public EHS_GlowConfig() : base(TypeName)
    {
    }
}
