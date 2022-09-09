
public class EHS_IntConfig : EHS_Config
{
    public static readonly string TypeName = "int";

    public int value;

    public EHS_IntConfig() : base(TypeName)
    {
    }

    public EHS_IntConfig(int value) : base(TypeName)
    {
        this.value = value;
    }
}
