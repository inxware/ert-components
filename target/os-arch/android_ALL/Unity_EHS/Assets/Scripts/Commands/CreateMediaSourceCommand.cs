
public class CreateMediaSourceCommand : Command
{
    public static readonly string TypeName = "createMediaSource";

    private EHS_MediaSourceConfig config;

    public CreateMediaSourceCommand(EHS_SceneManager manager, EHS_MediaSourceConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        return manager.CreateMediaSource(config);
    }
}
