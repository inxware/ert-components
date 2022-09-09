using UnityEngine;

public class RemoveObjectCommand : Command
{
    public static readonly string TypeName = "removeObject";
    private string id;

    public RemoveObjectCommand(EHS_SceneManager manager, string id) : base(manager)
    {
        this.id = id;
    }

    public override string Type()
    {
        return TypeName;
    }

    public override bool Exec()
    {
        Debug.Log("Destroy object id:" + id);
        manager.DestroyObject(id);
        return true;
    }
}
