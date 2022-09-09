
[System.Serializable]
public class EHS_EventCollectionConfig : EHS_Config
{
    public EHS_EventConfig [] collection;
    
    public EHS_EventCollectionConfig(string type) : base(type)
    {
    }
}
