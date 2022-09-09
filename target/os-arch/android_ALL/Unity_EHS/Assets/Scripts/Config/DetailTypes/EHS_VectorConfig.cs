
using UnityEngine;

public class EHS_VectorConfig : EHS_Config
{
    public static readonly string TypeName = "vector";

    public Vector3 value;

    public EHS_VectorConfig() : base(TypeName)
    {
    }

    public EHS_VectorConfig(Vector3 value) : base(TypeName)
    {
        this.value = value;
    }
}
