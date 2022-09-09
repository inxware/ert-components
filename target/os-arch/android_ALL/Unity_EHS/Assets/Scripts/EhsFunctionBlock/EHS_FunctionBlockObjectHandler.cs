
public abstract class EHS_FunctionBlockObjectHandler
{
    protected EHS_FunctionBlockManager manager;

    public EHS_FunctionBlockObjectHandler(EHS_FunctionBlockManager manager)
    {
        this.manager = manager;
    }

    public abstract bool Create(EHS_FBObject ehsFbObject);
    public abstract void Destroy(EHS_FBObject ehsFbObject);
    public abstract bool SetMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config);
    public abstract bool StopMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config);
}
