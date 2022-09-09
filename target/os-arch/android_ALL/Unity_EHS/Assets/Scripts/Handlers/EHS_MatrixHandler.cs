using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_MatrixHandler : EHS_ObjectHandler
{
    public EHS_MatrixHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        // (1) - create 3x3 matrix handler
        //objectDetailHandlers.Add(EHS_DetailType.Surface, new MatrixSurfaceHandler(manager)); // @TODO -fix later
        // (2) ...
        // ...
    }

    public static void ApplyImage(GameObject matrix, Texture2D [,] texture, string side, int size)
    {
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                string cubeName = "Cube_" + (i + 1) + "x" + (j + 1);
                GameObject cube = EHS_ObjectUtils.GetChildWithName(matrix, cubeName);
                GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, side);
                var mesh = quad.GetComponent<MeshRenderer>();
                var material = mesh.materials[0];
                material.mainTexture = texture[i,j];
            }
        }
    }
    /*
    public class MatrixSurfaceHandler : ObjectDetailHandler
    {
        public MatrixSurfaceHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all material handlers
            // (1) image media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Image, new ImageSourceMaterialDetailHandler(manager));
            // (2) video media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Video, new ImageSourceMaterialDetailHandler(manager));
            // (3) ...
        }

        public class ImageSourceMaterialDetailHandler : IObjectDetailValuesHandler
        {
            private static readonly int DETAIL_VALUE_PATH_INDEX = 1;
            private static readonly int DETAIL_VALUE_SIDE_INDEX = 2;

            private EHS_SceneManager manager;

            private int minValueCount = 2;

            public ImageSourceMaterialDetailHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            public bool Update(EHS_Object ehsObject, string [] values)
            {
                if(values != null && values.Length >= minValueCount){
                    try{
                        string type = values[DETAIL_VALUE_TYPE_INDEX];
                        string path = values[DETAIL_VALUE_PATH_INDEX];
                        string sides = "top,bottom,left,right,front,back";
                        if(values.Length == 3){ // check if a single or all sides get updated
                            sides = values[DETAIL_VALUE_SIDE_INDEX];
                        }
                        EHS_MediaSource source = ehsObject.GetMediaSourceHandler().GetMediaSource(type);
                        return source.Attach(ehsObject, EHS_MediaSourceConfig.Create(ehsObject.GetId(), type, path,
                                             MatrixTextureHandler.HandlerType, sides));
                    }catch(Exception e){
                        Debug.LogException(e);
                    }
                }else{
                    Debug.Log("Invalid value data passed to details (expecting minimum of " + minValueCount + " values).");
                }
                return false;
            }

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                return true;
            }
        }
    }

    public class MatrixTextureHandler : TextureHandler
    {
        public static readonly string HandlerType = "matrix";
        private static readonly string TEMP_MATRIX_TEXTURE = "matrix_3x3_video";

        public MatrixTextureHandler(EHS_MediaSource mediaSource) : base(mediaSource)
        {
        }

        public override bool Update(EHS_Object ehsObject, Texture texture, string[] sides)
        {
            int size = 3; // 3x3 matrix
            try{
                var textureContainer = ehsObject.GetTextureContainer();
                var texture2D = texture as Texture2D;
                if(texture2D == null){
                    if(texture == null){
                        Debug.Log("Failed to update matrix texture, as the texture source is null.");
                        return false;    
                    }
                    texture2D = textureContainer.Get(TEMP_MATRIX_TEXTURE);
                    if(texture2D != null){
                        EHS_TextureUtils.RenderTexture2Texture2D(texture as RenderTexture, texture2D);
                    }else{
                        texture2D = EHS_TextureUtils.RenderTexture2Texture2D(texture as RenderTexture);
                        textureContainer.Add(TEMP_MATRIX_TEXTURE, texture2D);
                    }
                    if(texture2D == null){
                        Debug.Log("Failed to update matrix texture, as the RenderTexture could not be converted to Texture2D.");
                    }
                }
                var textureGrid = textureContainer.GetGrid(texture2D);
                if(textureGrid != null){
                    textureGrid = EHS_TextureUtils.UpdateTextureGrid(texture2D, textureGrid);
                }else{
                    textureGrid = EHS_TextureUtils.CreateTextureGrid(texture2D, size);
                    textureContainer.AddGrid(texture2D, textureGrid);
                }
                foreach(string side in sides){
                    if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                        ApplyImage(ehsObject.GetGameObject(), textureGrid, EHS_ObjectUtils.QuadNameMap[side], size);
                    }
                }
                return true;
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }
    }
    */
}
