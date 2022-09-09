
public class EHS_FloatConfig : EHS_Config
{
    public static readonly string TypeName = "float";

    public float value;

    public EHS_FloatConfig() : base(TypeName)
    {
    }

    public EHS_FloatConfig(float value) : base(TypeName)
    {
        this.value = value;
    }
}
