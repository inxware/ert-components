using System;
using UnityEngine;

public class EHS_CommonDetailHandler : EHS_ObjectHandler
{
    public EHS_CommonDetailHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        // (1) - Material handler
        objectDetailHandlers.Add(EHS_DetailType.Material, new MaterialDetailHandler(manager));
        // (2) ...
    }

    /* Object Material Details Handler */
    public class MaterialDetailHandler : ObjectDetailHandler
    {
        public MaterialDetailHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all material handlers
            // (1) image media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Image, new ImageSourceMaterialDetailHandler(manager));
            // (2) video media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Video, new VideoSourceMaterialDetailHandler(manager));
        }

        public class ImageSourceMaterialDetailHandler : IObjectDetailValuesHandler
        {
            private EHS_SceneManager manager;

            public ImageSourceMaterialDetailHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                try{
                    string path = configs.GetStringConfig(EHS_Details.Path);
                    return EHS_TextureUtils.UpdateObjectTexture(ehsObject, path);
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class VideoSourceMaterialDetailHandler : IObjectDetailValuesHandler
        {
            private EHS_SceneManager manager;

            public VideoSourceMaterialDetailHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                try{
                    string type_id = id;
                    string path = configs.GetStringConfig(EHS_Details.Path);
                    EHS_MediaSource source = ehsObject.GetMediaSourceHandler().GetMediaSource(type_id);
                    return source.Attach(ehsObject, EHS_MediaSourceConfig.Create(ehsObject.GetId(), type_id, path));
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }
    }
}
