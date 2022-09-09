using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_QuadObjectSurfaceHandler : ObjectDetailHandler
 {
    public static readonly string AllSides = "all";

    public struct Config { 
        public string allSides;
        public float fontTextMeshScale;
        public float fontSizeScale;
    }

    private Config config;

    public EHS_QuadObjectSurfaceHandler(EHS_SceneManager manager, Config config) : base(manager)
    {
        this.config = config;
        // register all material handlers
        // (1) image media source
        objectDetailValuesHandlers.Add(EHS_MediaSourceType.Image, new MediaSourceMaterialDetailHandler(this));
        // (2) video media source
        objectDetailValuesHandlers.Add(EHS_MediaSourceType.Video, new MediaSourceMaterialDetailHandler(this));
        // (3) text
        objectDetailValuesHandlers.Add(EHS_DetailType.Text, new TextDetailHandler(this));
        // (4) background
        objectDetailValuesHandlers.Add(EHS_DetailType.View, new BackgroundDetailHandler(this));
        // (5) clear surface of media and text
        objectDetailValuesHandlers.Add(EHS_DetailType.Clear, new ClearSurfaceDetailHandler(this));
        // (6) clear surface of media and text
        objectDetailValuesHandlers.Add(EHS_DetailType.Fade, new FadeDetailHandler(this));
    }

    public abstract class QuadObjectSurfaceDetailValuesHandler : IObjectDetailValuesHandler
    {
        protected EHS_SceneManager manager;
        protected EHS_QuadObjectSurfaceHandler parent;

        public QuadObjectSurfaceDetailValuesHandler(EHS_QuadObjectSurfaceHandler parent)
        {
            this.parent = parent;
            this.manager = parent.manager;
        }

        protected abstract bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs);

        public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
            try{
                string sides = configs.GetStringConfig(EHS_Details.Side);
                if(string.IsNullOrEmpty(sides) || sides.Equals(AllSides)){
                    sides = parent.config.allSides;
                }
                ClearSurfaceText(ehsObject, sides);
                return UpdateSurface(ehsObject, /*type id*/id, sides, configs);
            }catch(Exception e){
                Debug.LogException(e);
            }
            return true;
        }

        protected void ClearSurfaceText(EHS_Object ehsObject, string sides)
        {
            try{
                var cube = ehsObject.GetGameObject();
                string[] sidesList = EHS_ObjectUtils.GetSides(sides);
                foreach(string side in sidesList){
                    if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                        string sideName = EHS_ObjectUtils.QuadNameMap[side];
                        GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, sideName);
                        var textObject = EHS_TextObjectFactory.Create(
                                            new EHS_TextObjectConfig()
                                            .SetParentObject(quad)
                                            .SetOnlyExisting(true));
                        if(textObject != null){ 
                            textObject.Clear();    
                        }
                    }
                }
            }catch(Exception e){
                Debug.LogException(e);
            }
        }

        protected void ClearTexture(EHS_Object ehsObject, string sides)
        {
            try{
                var cube = ehsObject.GetGameObject();
                string[] sidesList = EHS_ObjectUtils.GetSides(sides);
                foreach(string side in sidesList){
                    if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                        string sideName = EHS_ObjectUtils.QuadNameMap[side];
                        GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, sideName);
                        EHS_QuadHandler.ClearTexture(ehsObject, quad, sideName);
                    }
                }
            }catch(Exception e){
                Debug.LogException(e);
            }
        }
    }

    public class MediaSourceMaterialDetailHandler : QuadObjectSurfaceDetailValuesHandler
    {
        public MediaSourceMaterialDetailHandler(EHS_QuadObjectSurfaceHandler parent) : base(parent)
        {
        }

        protected override bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs){ 
            string path = configs.GetStringConfig(EHS_Details.Path);
            EHS_MediaSource source = ehsObject.GetMediaSourceHandler().GetMediaSource(type);
            return source.Attach(ehsObject, EHS_MediaSourceConfig.Create(ehsObject.GetId(), type, path,
                                    QuadObjectTextureHandler.HandlerType, sides));
        }
    }

    public class TextDetailHandler : QuadObjectSurfaceDetailValuesHandler
    {
        public TextDetailHandler(EHS_QuadObjectSurfaceHandler parent) : base(parent)
        {
        }

        protected override bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs){
            try{
                string[] sidesList = EHS_ObjectUtils.GetSides(sides);
                string text = configs.GetStringConfig(EHS_Details.Text,"");
                var textConfig = configs.GetConfig(EHS_TextConfig.TypeName) as EHS_TextConfig; 
                if(textConfig != null){
                    GameObject cube = ehsObject.GetGameObject();
                    GameObject prefab = manager.GetPrefab(EHS_ObjectType.Text);
                    foreach(string side in sidesList){
                        if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                            string sideName = EHS_ObjectUtils.QuadNameMap[side];
                            GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, sideName);
                            if(quad != null && prefab != null){
                                var textObject = EHS_TextObjectFactory.Create(
                                    new EHS_TextObjectConfig()
                                    .SetEhsObject(ehsObject)
                                    .SetParentObject(quad)
                                    .SetTextPrefab(prefab)
                                    .SetSide(side));
                                if(textObject != null){ 
                                    textObject.Update(textConfig, text);
                                }else{ 
                                    Debug.Log("Failed to attach text on the quad.");
                                    return false;    
                                }
                            }else{
                                Debug.Log("Failed to attach text on the quad.");
                                return false;
                            }
                        }
                    }
                    return true;
                }
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }
    }

    public class BackgroundDetailHandler : QuadObjectSurfaceDetailValuesHandler
    {
        public BackgroundDetailHandler(EHS_QuadObjectSurfaceHandler parent) : base(parent)
        {
        }

        protected override bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs){
            try{
                string bgrColor = configs.GetStringConfig(EHS_Details.BgrColor,null);
                string fgrColor = configs.GetStringConfig(EHS_Details.FgrColor,null);
                if(string.IsNullOrEmpty(bgrColor)){ 
                    return false;    
                }
                return UpdateColors(ehsObject, sides, bgrColor, fgrColor);
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }

        public static bool UpdateColors(EHS_Object ehsObject, string sides, string bgrColor, string fgrColor){ 
            string[] sidesList = EHS_ObjectUtils.GetSides(sides);
            GameObject cube = ehsObject.GetGameObject();
            foreach(string side in sidesList){
                if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                    string sideName = EHS_ObjectUtils.QuadNameMap[side];
                    GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, sideName);
                    if(quad != null){
                        EHS_QuadHandler.UpdateColor(ehsObject, quad, bgrColor, fgrColor, sideName);
                    }else{
                        Debug.Log("Failed to attach text on the quad(cube).");
                        return false;
                    }
                }
            }
            return true;
        }
    }

    public class ClearSurfaceDetailHandler : QuadObjectSurfaceDetailValuesHandler {
        public ClearSurfaceDetailHandler(EHS_QuadObjectSurfaceHandler parent) : base(parent)
        {
        }

        protected override bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs){ 
            try{
                string surfaceObject = configs.GetStringConfig(EHS_Details.Object);
                if(!string.IsNullOrEmpty(surfaceObject)){
                    if(surfaceObject.Equals("texture")){ 
                        ClearTexture(ehsObject, sides);
                        return true;
                    }else if(surfaceObject.Contains("|text")){ 
                        ClearSurfaceText(ehsObject, sides);
                    }
                    bool mediaContinue = configs.GetBoolConfig(EHS_Details.MediaContinue);
                    return EHS_QuadHandler.ClearMedia(ehsObject, mediaContinue, surfaceObject, sides);
                }
            }catch(Exception e){ 
                Debug.LogException(e);
            }
            return true;
        }
    }

    public class FadeDetailHandler : QuadObjectSurfaceDetailValuesHandler
    {
        public FadeDetailHandler(EHS_QuadObjectSurfaceHandler parent) : base(parent)
        {
        }

        protected override bool UpdateSurface(EHS_Object ehsObject, string type, string sides, EHS_ConfigList configs){ 
            try{
                string bgrColor = configs.GetStringConfig(EHS_Details.BgrColor);
                string fgrColor = configs.GetStringConfig(EHS_Details.FgrColor);
                string fadeDirection = configs.GetStringConfig("direction");
                float fadeSpeed = configs.GetFloatConfig("fadeSpeed");
                    
                manager.StopCoroutine(Fading(sides, ehsObject,  bgrColor, fgrColor, fadeSpeed, fadeDirection));
                if(!string.IsNullOrEmpty(fadeDirection) && fadeSpeed > 0f){ 
                    manager.StartCoroutine(Fading(sides, ehsObject,  bgrColor, fgrColor, fadeSpeed, fadeDirection));
                }else{
                    BackgroundDetailHandler.UpdateColors(ehsObject, sides, bgrColor, fgrColor);
                }
            }catch(Exception e){ 
                Debug.LogException(e);
            }
            return true;
        }

        IEnumerator Fading(string sides, EHS_Object ehsObject, string bgrColor, string fgrColor, float fadeSpeed, string direction) 
        { 
            string[] sidesList = EHS_ObjectUtils.GetSides(sides);
            var updaters = new Dictionary<string, EHS_QuadHandler.QuadSurfaceHandler.FadeDetailHandler.FadingUpdater>();
            bool isUpdateFailed = false;
            // create fading updaters
            GameObject cube = ehsObject.GetGameObject();
            foreach(string side in sidesList){
                if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                    string sideName = EHS_ObjectUtils.QuadNameMap[side];
                    GameObject quad = EHS_ObjectUtils.GetChildWithName(cube, sideName);
                    if(quad != null){
                        var updater = new EHS_QuadHandler.QuadSurfaceHandler.FadeDetailHandler.
                            FadingUpdater(sideName, ehsObject, quad, direction, bgrColor, fgrColor, fadeSpeed);
                        updaters.Add(side, updater);
                    }else{
                        Debug.LogWarning("Failed to create a fading effect on the quad(cube).");
                        isUpdateFailed = true;
                        break;
                    }
                }
            }
            // apply fading
            if(!isUpdateFailed && (updaters.Count > 0)){
                int completedCount = 0;
                while(completedCount != updaters.Count){
                    foreach(var updaterItem in updaters){
                        var updater = updaterItem.Value;
                        updater.Update();
                        if(updater.IsUpdateCompleted()){
                            completedCount++;
                        }
                    }
                    yield return null;    
                }
                ehsObject.UpdateState(EHS_DetailType.Fade + "_" + direction, true);
            }else{
                ehsObject.UpdateState(EHS_DetailType.Fade + "_" +  direction, false);
            }
        }
    }

    public class QuadObjectTextureHandler : TextureHandler
    {
        public static readonly string HandlerType = "quadobject"; 
        public QuadObjectTextureHandler(EHS_MediaSource mediaSource) : base(mediaSource)
        {
        }

        public override bool Update(EHS_Object ehsObject, Texture texture, string[] sides)
        {
            try{
                foreach(string side in sides){
                    if(EHS_ObjectUtils.QuadNameMap.ContainsKey(side)){
                        EHS_MaterialUtils.ApplyImage(ehsObject, texture, EHS_ObjectUtils.QuadNameMap[side]);
                    }
                }
                return true;
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }
    }
}