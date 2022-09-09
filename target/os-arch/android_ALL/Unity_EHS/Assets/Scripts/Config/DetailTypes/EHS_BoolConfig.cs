public class EHS_BoolConfig : EHS_Config
{
    public static readonly string TypeName = "bool";

    public bool value;

    public EHS_BoolConfig() : base(TypeName)
    {
    }

    public EHS_BoolConfig(bool value) : base(TypeName)
    {
        this.value = value;
    }
}
