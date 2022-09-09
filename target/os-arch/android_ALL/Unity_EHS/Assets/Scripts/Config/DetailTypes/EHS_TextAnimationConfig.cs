using System.Collections.Generic;

[System.Serializable]
public class EHS_TextAnimationConfig : EHS_Config
{
    [System.Serializable]
    public struct AnimationModifier { 
        public string name; // name of the animation
        public string modifier;
    }

    public static readonly string TypeName = "textAnimationConfig";
    
    public List<string> appearAnimations;
    public List<string> behaviourAnimations;
    public List<AnimationModifier> appearanceModifiers;
    public List<AnimationModifier> behaviourModifiers;
    public bool useTypeWriter = true;
    public EHS_TextAnimationConfig() : base(TypeName)
    {
    }
}