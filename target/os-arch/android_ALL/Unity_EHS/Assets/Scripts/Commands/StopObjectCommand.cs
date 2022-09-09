using UnityEngine;

public class StopObjectCommand : Command
{
    public static readonly string TypeName = "stopObject";

    private EHS_ObjectConfig config;

    public StopObjectCommand(EHS_SceneManager manager, EHS_ObjectConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        return manager.StopObject(config);
    }
}
