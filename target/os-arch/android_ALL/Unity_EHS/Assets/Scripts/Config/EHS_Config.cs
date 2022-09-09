
[System.Serializable]
public class EHS_Config
{
    public string type;
    public string id;

    public EHS_Config(string type)
    {
        this.type = type;
    }

    public EHS_Config SetId(string id){
        this.id = id;
        return this;    
    }
}
