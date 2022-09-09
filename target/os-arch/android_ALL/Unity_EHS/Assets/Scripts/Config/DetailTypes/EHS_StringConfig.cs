
public class EHS_StringConfig : EHS_Config
{
    public static readonly string TypeName = "string";

    public string value;

    public EHS_StringConfig() : base(TypeName)
    {
    }

    public EHS_StringConfig(string value) : base(TypeName)
    {
        this.value = value;
    }
}
