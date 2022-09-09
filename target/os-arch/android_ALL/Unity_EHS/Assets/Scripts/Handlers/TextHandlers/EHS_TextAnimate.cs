
using Febucci.UI;
using System.Collections.Generic;
using TMPro;

public class EHS_TextAnimate 
{
    private static List<string> Appearances = new List<string>(TAnimTags.defaultAppearances);
    private static List<string> Behaviours = new List<string>(TAnimTags.defaultBehaviors);

    // Tune the default modifiers
    // note : appearence seems to be fine with the signage
    private static Dictionary<string, string> AppearanceModifiers = new Dictionary<string, string>(){ 
        { "size", "" },
        { "fade", "" },
        { "offset", "" },
        { "vertexp", "" },
        { "horiexp", "" },
        { "diagexp", "" },
        { "rot", "" }
    };
    private static Dictionary<string, string> BehaviourModifiers = new Dictionary<string, string>(){
        { "wiggle", "a=0.0015 f=3.5" },
        { "shake", "a=0.001 d=0.04" },
        { "wave", "a=0.005 f=1.78 w=3" },
        { "slide", "a=0.001 f=4 w=0" },
        { "bounce", "a=0.08 f=1 w=0.08" },
        { "rot", "f=1 w=10" },
        { "swing", "a=1 f=3 w=0" },
        { "incr", "a=1.4 f=4.84 w=0.18" },
        { "rainb", "f=0.8 w=0.08" },
        { "fade", "d=1.2" },
        { "dangle", "a=0.0035 f=2.41 w=0.1" },
        { "pend", "a=0.4 f=3 w=0.2" }
    };

    private TextAnimatorPlayer textAnimatorPlayer;
    private TextAnimator textAnimator;
    private List<string> appearances;
    private List<string> behaviours;
    private Dictionary<string, string> appearanceModifiers;
    private Dictionary<string, string> behaviourModifiers;
    public bool Enabled { get; private set; }
    
    public EHS_TextAnimate(TextMeshPro textMesh){
        appearances = new List<string>();
        behaviours = new List<string>();
        appearanceModifiers = AppearanceModifiers;
        behaviourModifiers = BehaviourModifiers;
        textAnimatorPlayer = textMesh.GetComponent<TextAnimatorPlayer>();
        textAnimator = textMesh.GetComponent<TextAnimator>();
        textAnimatorPlayer.useTypeWriter = false;
        textAnimatorPlayer.enabled = true;
        textAnimator.enabled = true;
        Enabled = true;
        if(textAnimatorPlayer == null || textAnimator == null){ 
            throw new System.Exception("Unable to create Text animation as there's missing TextAnimatorPlayer and TextAnimator in TMPro Text object.");    
        }
    }

    public void SetAnimation(EHS_TextAnimationConfig animationConfig){ 
        if(animationConfig != null && HasAnimations(animationConfig)){
            Enabled = true;
            ApplyAnimations(animationConfig);
        }else{ 
            Disable();
        }
    }

    public void SetText(string text){ 
        textAnimatorPlayer.ShowText( AddEffects( text ) );
    }

    public void Disable(){ 
        appearances.Clear();
        behaviours.Clear();
        if(Enabled){ 
            textAnimatorPlayer.useTypeWriter = false;
            Enabled = false;    
        }
    }

    private bool HasAnimations(EHS_TextAnimationConfig animationConfig){ 
        
        return (animationConfig.appearAnimations != null && animationConfig.appearAnimations.Count > 0) ||
               (animationConfig.behaviourAnimations != null && animationConfig.behaviourAnimations.Count > 0);
    }

    private void ApplyAnimations(EHS_TextAnimationConfig animationConfig){
        if(animationConfig.appearAnimations != null){
            appearances = animationConfig.appearAnimations;
        }else{ 
            appearances.Clear();    
        }
        if(animationConfig.behaviourAnimations != null){
            behaviours = animationConfig.behaviourAnimations;
        }else{ 
            behaviours.Clear();    
        }
        if(animationConfig.useTypeWriter != textAnimatorPlayer.useTypeWriter){ 
            textAnimatorPlayer.useTypeWriter = animationConfig.useTypeWriter;
        }

        if(animationConfig.appearanceModifiers != null){ 
            foreach(var modifier in animationConfig.appearanceModifiers){ 
                if(!string.IsNullOrEmpty(modifier.name) && appearanceModifiers.ContainsKey(modifier.name)){ 
                    appearanceModifiers[modifier.name] = modifier.modifier;
                }
            }    
        }

        if(animationConfig.behaviourModifiers != null){ 
            foreach(var modifier in animationConfig.behaviourModifiers){ 
                if(!string.IsNullOrEmpty(modifier.name) && behaviourModifiers.ContainsKey(modifier.name)){ 
                    behaviourModifiers[modifier.name] = modifier.modifier;
                }
            }    
        }
    }

    private string AddEffects(string text){
        if(!string.IsNullOrEmpty(text) && Enabled){ 
            foreach(var effect in appearances){ 
                if(Appearances.Contains(effect)){ 
                    text = "{"+ModifyAppearances(effect)+"}" + text;
                }else{ 
                    EHS_Utils.Log("Attept to apply unknown appearence effect ("+effect+")");
                }
            }
            foreach(var effect in behaviours){ 
                if(Behaviours.Contains(effect)){ 
                    text = "<"+ModifyBehaviours(effect)+">" + text;
                }else{ 
                    EHS_Utils.Log("Attept to apply unknown behaviour effect ("+effect+")");
                }
            }

            foreach(var effect in behaviours){ 
                if(Behaviours.Contains(effect)){ 
                    text = text + "</"+effect+">";
                }
            }
            foreach(var effect in appearances){ 
                if(Appearances.Contains(effect)){ 
                    text = text + "{/"+effect+"}";
                }
            }
        }
        return text;
    }

    private string ModifyAppearances(string effect){ 
        if(appearanceModifiers.ContainsKey(effect)){
            string modifier = appearanceModifiers[effect];
            if(!string.IsNullOrEmpty(modifier)){
                effect = effect + " " + modifier;
            }
        }
        return effect;    
    }

    private string ModifyBehaviours(string effect){ 
        if(behaviourModifiers.ContainsKey(effect)){ 
            string modifier = behaviourModifiers[effect];
            if(!string.IsNullOrEmpty(modifier)){
                effect = effect + " " + modifier;
            }
        }
        return effect;    
    }
}
