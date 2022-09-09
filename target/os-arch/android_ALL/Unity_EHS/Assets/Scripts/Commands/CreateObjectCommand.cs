using UnityEngine;

public class CreateObjectCommand : Command
{
    public static readonly string TypeName = "createObject";

    private EHS_ObjectConfig config;

    public CreateObjectCommand(EHS_SceneManager manager, EHS_ObjectConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        return manager.CreateObject(config) != null;
    }
}
