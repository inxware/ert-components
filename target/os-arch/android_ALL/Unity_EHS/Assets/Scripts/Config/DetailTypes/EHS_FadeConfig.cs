
using UnityEngine;

[System.Serializable]
public class EHS_FadeConfig  : EHS_Config
{
    public static readonly string TypeName = "fadeConfig";

    public int steps = 10;
    public float delay = 0.05f;

    public EHS_FadeConfig() : base(TypeName)
    {
    }
}