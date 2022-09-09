using System;
using UnityEngine;

public class EHS_PresentationHandler : EHS_ObjectHandler
{
    public static readonly string CreateDetail = "create";

    public EHS_PresentationHandler(EHS_SceneManager manager) : base(manager)
    {
        // register presentation handlers
        // (1)
        objectDetailHandlers.Add(EHS_PresentationType.Background, new PresentationDetailHandler<EHS_BackgroundPresentation>(manager));
        // (2)
        objectDetailHandlers.Add(EHS_PresentationType.Quad, new PresentationDetailHandler<EHS_QuadPresentation>(manager));
        // (3)
        objectDetailHandlers.Add(EHS_PresentationType.Cube, new PresentationDetailHandler<EHS_CubePresentation>(manager));
        // (4)
        objectDetailHandlers.Add(EHS_PresentationType.Audio, new PresentationDetailHandler<EHS_AudioPresentation>(manager));
        // ...
    }

    public class PresentationDestroyListener : IObjectDestroyListener { 
                
        private EHS_Presentation presentation;
        private IObjectDestroyListener destroyListener;

        public PresentationDestroyListener(EHS_Presentation presentation){ 
            this.presentation = presentation;
        }

        public void OnDestroy(EHS_Object ehsObject){ 
            this.presentation.OnPresentationDestroy(ehsObject);
            if(destroyListener != null){
                // destroy any other things which need to be destroyed
                // after presentation has been destroyed
                destroyListener.OnDestroy(ehsObject);    
            }
        }

        public void SetDestroyListener(IObjectDestroyListener destroyListener){ 
            this.destroyListener = destroyListener;
        }
    }

    /* Object Details Handler */
    public class PresentationDetailHandler<T> : ObjectDetailHandler where T : EHS_Presentation
    {
        public PresentationDetailHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all handlers
            // (0) - special detail for a new presetation
            objectDetailValuesHandlers.Add(EHS_PresentationHandler.CreateDetail, new UpdatePresentationDetailHandler<T>(manager));
            // (1)
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Image, new UpdatePresentationDetailHandler<T>(manager));
            // (2)
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Video, new UpdatePresentationDetailHandler<T>(manager));
            // (3)
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Audio, new UpdatePresentationDetailHandler<T>(manager));
            // (4)
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.CGI, new UpdatePresentationDetailHandler<T>(manager));
            // (5)
            objectDetailValuesHandlers.Add(EHS_ObjectType.Text, new UpdatePresentationDetailHandler<T>(manager));
        }

        public override bool Stop(EHS_Object ehsObject)
        {
            try{
                var unityObject = ehsObject.GetGameObject();
                var presentation = unityObject.GetComponent<T>();
                if(presentation != null){
                    presentation.StopMedia();
                }                    
                return true;
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }

        public class UpdatePresentationDetailHandler<D> : IObjectDetailValuesHandler where D : EHS_Presentation
        {   
            private EHS_SceneManager manager;

            public UpdatePresentationDetailHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            private bool Create(EHS_Object ehsObject, string type, EHS_PresentationConfig config){ 
                try{
                    string contentType = type;
                    if(string.IsNullOrEmpty(contentType)){ 
                        Debug.LogWarning("Cannot create presentation for empty content type.");
                        return false;
                    }
                    ehsObject.SetMetaData("contentType", contentType);
                    config.id = ehsObject.GetId();
                    var unityObject = ehsObject.GetGameObject();
                    if(!unityObject.activeSelf){
                        unityObject.SetActive(true);
                    }
                    var presentation = unityObject.GetComponent<D>();
                    if(presentation == null){
                        presentation = unityObject.AddComponent<D>();
                    }
                    ehsObject.SetCallBack(new PresentationDestroyListener(presentation));
                    presentation.Initialise(config, manager);
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }

            private bool SetMedia(EHS_Object ehsObject, EHS_PresentationConfig config){
                try{
                    config.id = ehsObject.GetId();
                    var unityObject = ehsObject.GetGameObject();
                    if(!unityObject.activeSelf){
                        unityObject.SetActive(true);
                    }
                    var presentation = unityObject.GetComponent<D>();
                    presentation.SetMedia(config);
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return true;
            }

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) 
            {
                try{
                    if(id.Equals(EHS_PresentationHandler.CreateDetail)){
                        var config = configs.GetConfig(type) as EHS_PresentationConfig;
                        return Create(ehsObject, type, config);
                    }else{
                        var config = configs.GetConfig(id) as EHS_PresentationConfig;
                        return SetMedia(ehsObject, config);
                    }
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }
    }
}
