
[System.Serializable]
public class EHS_AudioConfig  : EHS_Config
{
    public static readonly string TypeName = "audioConfig";

    public bool looping = false;
    public float volume = 1f;
    public float pitch = 1f;

    public EHS_AudioConfig() : base(TypeName)
    {
    }
}