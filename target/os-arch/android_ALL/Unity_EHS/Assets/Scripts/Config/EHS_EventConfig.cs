
using UnityEngine;

[System.Serializable]
public class EHS_EventConfig : EHS_Config
{
    public Vector3 direction;
    public Vector3 boundries;
    public Vector3 freeze = Vector3.zero;
    public float speed = 0f;
    public float steps = 0f;
    public float targetError = 0f;
    public bool destroy = false; // set this to true to destroy object on complete 
    public bool notifyOnComplete = false;
    public string [] details;
    public EHS_ConfigList metadata;

    public static EHS_EventConfig CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<EHS_EventConfig>(jsonString);
    }

    public EHS_EventConfig(string type) : base(type)
    {
    }
}
