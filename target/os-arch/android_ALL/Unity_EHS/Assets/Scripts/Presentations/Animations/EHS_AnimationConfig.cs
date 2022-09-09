using UnityEngine;

[System.Serializable]
public class EHS_AnimationConfig
{
    public string type;
    // transition
    public float speed = 0f;
    public float steps = 0f;
    public string direction;

    // update
    public bool update = false;

    public EHS_AnimationConfig(){ }

    public EHS_AnimationConfig(string type){
        this.type = type;
    }

    public EHS_AnimationConfig Copy(){ 
        return (EHS_AnimationConfig)this.MemberwiseClone();
    }
}
