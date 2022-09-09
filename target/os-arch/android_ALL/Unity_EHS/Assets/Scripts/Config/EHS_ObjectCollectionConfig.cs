
[System.Serializable]
public class EHS_ObjectCollectionConfig : EHS_ObjectConfig
{
    public EHS_ObjectConfig [] collection;

    public EHS_ObjectCollectionConfig(string type) : base(type)
    {
    }
}
