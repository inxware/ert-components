
using UnityEngine;

public class EHS_CubePresentation : EHS_Presentation
{
    protected override EHS_PresentationObject CreatePresentationObject(EHS_PresentationConfig config){ 
        return new CubePresentationObject(config.id, this);     
    }
    
    public class CubePresentationObject : EHS_PresentationObject {

        public CubePresentationObject(string id, EHS_Presentation presentation) : base(id, EHS_PresentationType.Cube, EHS_ObjectType.QuadCube, presentation){
            
        }

        // create a default 'create' animation for the cube presentation
        protected override EHS_ObjectCreateAnimation DefaultObjectCreateAnimation(){ 
            var animation = animationFactory.CreateObjectCreateAnimation(new EHS_AnimationConfig(EHS_FadeInAnimation.Name));
            return animation;
        }

        // create a default 'update media' animation for the cube presentation
        protected override EHS_ObjectUpdateAnimation DefaultObjectUpdateAnimation(){ 
            var animation = animationFactory.CreateObjectUpdateAnimation(new EHS_AnimationConfig(EHS_RotatingAnimation.Name));
            return animation;
        }

        // create a default 'destroy' animation for the cube presentation
        protected override EHS_ObjectDestroyAnimation DefaultObjectDestroyAnimation(){ 
            var animation = animationFactory.CreateObjectDestroyAnimation(new EHS_AnimationConfig(EHS_FadeOutAnimation.Name));
            return animation;
        }

        protected override void OnCreate(EHS_PresentationConfig config){
            Debug.LogWarning("Create Cube Presentation");
            // init cube attributes
            scale = 3f*config.scale;
            position = config.position;
            rotation = Quaternion.identity;
            rotation.y = 0f;
            if(objectUpdateAnimation != null){ 
                objectUpdateAnimation.initalSide = "back";    
            }
        }
        
        protected override void OnStop(){
            // do nothing
        }

        protected override void OnDestroy(){
            // do nothing
        }
    }
}
