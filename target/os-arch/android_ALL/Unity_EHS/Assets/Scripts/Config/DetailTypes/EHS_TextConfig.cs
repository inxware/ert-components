using UnityEngine;

[System.Serializable]
public class EHS_TextConfig : EHS_Config
{
    public static readonly string TypeName = "textConfig";

    public string alignment = "center";
    public string fontStyle = "normal";
    public bool enableWrapping = true;
    public string overflowMode = "ellipsis";
    public string fontType = "arial";
    public string color = "255.255.255.255";
    public float fontScale = 1f;
    public float fontSize = 1f; // depricated
    public float margin = 0f;
    public EHS_GlowConfig glowConfig;
    public EHS_TextAnimationConfig animationConfig;

    // update
    public bool update = false; 

    public EHS_TextConfig() : base(TypeName)
    {
    }
}
