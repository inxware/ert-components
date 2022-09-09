
public class AttachEventCommand : Command
{
    public static readonly string TypeName = "attachEvent";

    private EHS_EventConfig config;

    public AttachEventCommand(EHS_SceneManager manager, EHS_EventConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        return manager.AttachEvent(config);
    }
}
