using UnityEngine;

public class EHS_QuadPresentation : EHS_Presentation
{
    protected override EHS_PresentationObject CreatePresentationObject(EHS_PresentationConfig config){ 
        return new QuadPresentationObject(config.id, this);     
    }

    public class QuadPresentationObject : EHS_PresentationObject {
        
        public QuadPresentationObject(string id, EHS_Presentation presentation) : base(id, EHS_PresentationType.Quad, EHS_ObjectType.QuadDuo, presentation){
            
        }

        // create a default 'create' animation for the presentation
        protected override EHS_ObjectCreateAnimation DefaultObjectCreateAnimation(){ 
            var animation = animationFactory.CreateObjectCreateAnimation(new EHS_AnimationConfig(EHS_FadeInAnimation.Name));
            return animation;
        }

        // create a default 'update media' animation for the presentation
        protected override EHS_ObjectUpdateAnimation DefaultObjectUpdateAnimation(){ 
            var animation = animationFactory.CreateObjectUpdateAnimation(new EHS_AnimationConfig(EHS_NoAnimation.Name));
            return animation;
        }

        // create a default 'destroy' animation for the presentation
        protected override EHS_ObjectDestroyAnimation DefaultObjectDestroyAnimation(){ 
            var animation = animationFactory.CreateObjectDestroyAnimation(new EHS_AnimationConfig(EHS_FadeOutAnimation.Name));
            return animation;
        }

        protected override void OnCreate(EHS_PresentationConfig config){
            Debug.LogWarning("Create Quad Presentation");
            Rect zone = config.zoneConfig.ToRect();
            scale.x = zone.width;
            scale.y = zone.height;
        }

        protected override void OnStop(){
            // do nothing
        }

        protected override void OnDestroy(){
            // do nothing
        }

        public void ClearText(){ 
            var command = new EHS_CommandConfig("updateObject");
            command.objectConfig = new EHS_ObjectConfig(objectType);
            command.objectConfig.id = GetObjectID();
            command.objectConfig.keep = true;
            command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                                .setConfigID(EHS_DetailType.Clear)
                                .appendConfig(EHS_Details.Side, EHS_QuadObjectSurfaceHandler.AllSides)
                                .appendConfig(EHS_Details.Object, "|text"));
            AddCommand(command);
        }
    }
}
