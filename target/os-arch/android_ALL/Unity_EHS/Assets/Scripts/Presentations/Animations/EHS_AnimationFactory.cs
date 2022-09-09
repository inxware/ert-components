
using System;
using UnityEngine;

public class EHS_AnimationFactory
{
    private EHS_Presentation presentation;
    private string objectType;
    private string id;

    public EHS_AnimationFactory(EHS_Presentation presentation, string id, string objectType){
        this.presentation = presentation;
        this.objectType = objectType;
        this.id = id;
    }

    // Object Update Animation
    public EHS_ObjectUpdateAnimation CreateObjectUpdateAnimation(EHS_AnimationConfig config){ 
        try{
            if(config != null && config.type != null){
                if(config.type.Equals(EHS_NoAnimation.Name)){ 
                    return new EHS_NoAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_RotatingAnimation.Name)){ 
                        return new EHS_RotatingAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_FadingAnimation.Name)){ 
                    return new EHS_FadingAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_InflatingAnimation.Name)){ 
                    return new EHS_InflatingAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_SlidingAnimation.Name)){ 
                    return new EHS_SlidingAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_ScrollAnimation.Name)){ 
                    return new EHS_ScrollAnimation(presentation, id, objectType);
                }else{ 
                    Debug.LogWarning("Unknown 'update' animation type: "+config.type);    
                }
            }
        }catch(Exception e){ 
            Debug.LogException(e);
        }
        return null;   
    }

    // Object Create Animation
    public EHS_ObjectCreateAnimation CreateObjectCreateAnimation(EHS_AnimationConfig config){ 
       try{
            if(config != null && config.type != null){
                if(config.type.Equals(EHS_FadeInAnimation.Name)){ 
                    return new EHS_FadeInAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_SlideInAnimation.Name)){ 
                    return new EHS_SlideInAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_InflateAnimation.Name)){ 
                    return new EHS_InflateAnimation(presentation, id, objectType);
                }else{ 
                    Debug.LogWarning("Unknown 'create' animation type: "+config.type);    
                }
            }
        }catch(Exception e){ 
            Debug.LogException(e);
        }
        return null;  
    }

    // Object Destroy Animation
    public EHS_ObjectDestroyAnimation CreateObjectDestroyAnimation(EHS_AnimationConfig config){ 
        try{
            if(config != null && config.type != null){
                if(config.type.Equals(EHS_FadeOutAnimation.Name)){ 
                    return new EHS_FadeOutAnimation(presentation, id, objectType); 
                }else if(config.type.Equals(EHS_SlideOutAnimation.Name)){ 
                    return new EHS_SlideOutAnimation(presentation, id, objectType);
                }else if(config.type.Equals(EHS_DeflateAnimation.Name)){ 
                    return new EHS_DeflateAnimation(presentation, id, objectType);
                }else{ 
                    Debug.LogWarning("Unknown 'destroy' animation type: "+config.type);    
                }
            }
        }catch(Exception e){ 
            Debug.LogException(e);
        }
        return null; 
    }
}
