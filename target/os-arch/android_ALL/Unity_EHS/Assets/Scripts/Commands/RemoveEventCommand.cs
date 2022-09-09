

public class RemoveEventCommand : Command
{
    public static readonly string TypeName = "removeEvent";

    private EHS_EventConfig config;

    public RemoveEventCommand(EHS_SceneManager manager, EHS_EventConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        return manager.RemoveEvent(config);
    }
}
