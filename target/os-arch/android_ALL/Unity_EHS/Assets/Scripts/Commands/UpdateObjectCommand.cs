using UnityEngine;

public class UpdateObjectCommand : Command
{
    public static readonly string TypeName = "updateObject";

    private EHS_ObjectConfig config;

    public UpdateObjectCommand(EHS_SceneManager manager, EHS_ObjectConfig config) : base(manager)
    {
        this.config = config;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        manager.UpdateObject(config);
        return true;
    }
}
