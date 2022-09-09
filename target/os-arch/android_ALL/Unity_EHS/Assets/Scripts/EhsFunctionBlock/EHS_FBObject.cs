
public class EHS_FBObject
{
    private EHS_FunctionBlockConfig config;
    private bool recreate = false;

    public EHS_FBObject(EHS_FunctionBlockConfig config)
    {
        this.config = config;
        this.recreate = false;
    }

    public EHS_FBObject(EHS_FunctionBlockConfig config, bool recreate)
    {
        this.config = config;
        this.recreate = recreate;
    }

    public string GetId()
    {
        return config.id;
    }

    public string GetCmd()
    {
        return config.cmd;
    }

    public string GetFBType()
    {
        return config.type;
    }

    public string GetPath()
    {
        return config.path;
    }

    public string GetText()
    {
        return config.text;
    }

    public bool ReCreate(){ 
        return recreate;
    }

    public EHS_FunctionBlockParametersConfig GetParameters()
    {
        return config.parameters;
    }
}
