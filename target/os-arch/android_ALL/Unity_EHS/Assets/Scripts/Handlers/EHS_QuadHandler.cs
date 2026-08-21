
using System;
using System.Collections;
using UnityEngine;

// @TODO- old handler. should be removed. any static function that is used here, hould be moved to material utils class
public class EHS_QuadHandler : EHS_ObjectHandler
{

    public static void ApplyImage(EHS_Object ehsObject, GameObject quad, Texture texture, string sufixId, bool parentScale=false)
    {
        var material = GetMaterial(ehsObject, quad, sufixId);
        ApplyImage(quad, material, texture, parentScale);
    }

    public static void ApplyImage(EHS_Object ehsObject, Texture texture, string sufixId, bool parentScale=false)
    {
        GameObject quad = ehsObject.GetGameObject();
        ApplyImage(ehsObject, quad, texture, sufixId, parentScale);
    }

    public static void ApplyImage(EHS_Object ehsObject, Texture texture, bool parentScale=false)
    {
        ApplyImage(ehsObject, texture, ehsObject.GetId(), parentScale);
    }

    public static void ApplyImage(GameObject quad, Material material, Texture texture, bool parentScale=false)
    {
        material.mainTexture = texture;
        // prevent streaching of the text
        float quadScale;
        if(parentScale){ 
            var transform = quad.transform.parent;
            quadScale = transform.localScale.x/transform.localScale.y;
        }else{
            quadScale = quad.transform.localScale.x/quad.transform.localScale.y;
        }
        float textureScale = (float)texture.width/(float)texture.height;
        if(quadScale != 1f || textureScale != 1f){
            float heigth = 1f, width = 1f;

            if(quadScale > textureScale){
                width = quadScale/textureScale;
            }else{ 
                heigth = textureScale/quadScale;
            }

            float offsetX = (1f - width)/2f;
            float offsetY = (1f - heigth)/2f;
            material.SetTextureScale("_MainTex", new Vector2(width, heigth));
            material.SetTextureOffset("_MainTex", new Vector2(offsetX, offsetY));
        }else{ 
            material.SetTextureScale("_MainTex", new Vector2(1f, 1f));
            material.SetTextureOffset("_MainTex", new Vector2(0f, 0f)); 
        }
    }

    public static void ClearTexture(EHS_Object ehsObject, GameObject unityObject, string sufixId){
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetTextureScale("_MainTex", new Vector2(0, 0));
        material.SetTextureOffset("_MainTex", new Vector2(0, 0));
    }

    public static Material GetMaterial(EHS_Object ehsObject, GameObject unityObject, string sufixId){ 
        var renderer = unityObject.GetComponent<Renderer>();
        var materialContainer = ehsObject.GetMaterialContainer();
        string materialKey = ehsObject.GetId() + "_BGRD_Color_" + sufixId;
        Material material = materialContainer.Get(materialKey);
        if(material == null){  // create new material
            material = new Material(renderer.sharedMaterial);
            materialContainer.Add(materialKey, material);
            renderer.sharedMaterial = material;
        }
        return material;
    }

    public static void UpdateBackgroundColor(EHS_Object ehsObject, GameObject unityObject, Color color, string sufixId){
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetColor("_BackgroundColor", color);
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color, string sufixId){ 
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetColor("_Color", color);
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color){
        UpdateForegroundColor(ehsObject, unityObject, color, "");
    }

    public static void UpdateBackgroundColor(EHS_Object ehsObject, GameObject unityObject, Color color){ 
        UpdateBackgroundColor(ehsObject, unityObject, color, "");
    }

    public static void UpdateColor(EHS_Object ehsObject, GameObject unityObject, string bgrColor, string fgrColor, string sufixId){ 
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        if(!string.IsNullOrEmpty(bgrColor)){
            Color color = EHS_Utils.PareseColor(bgrColor, Color.white);
            material.SetColor("_BackgroundColor", color);
        }
        if(!string.IsNullOrEmpty(fgrColor)){
            Color color = EHS_Utils.PareseColor(fgrColor, Color.white);
            material.SetColor("_Color", color);
        }
    }

    public static bool ClearMedia(EHS_Object ehsObject, bool mediaContinue, string details, string sides){
        try{
            string video = null; // @TODO - handle image if necessary
            string [] media = details.Split('|');
            foreach(var item in media){
                if(string.IsNullOrEmpty(item)){
                    continue;    
                }
                string [] content = item.Split(',');
                if(content[0].Equals(EHS_MediaSourceType.Video)){ 
                    video = content[1];
                }
            }
            //ClearTexture(ehsObject, ehsObject.GetGameObject(), ehsObject.GetId());
            if(!string.IsNullOrEmpty(video)){ 
                var source = (EHS_VideoSource)ehsObject.GetMediaSourceHandler().GetMediaSource(EHS_MediaSourceType.Video);
                if(source != null){
                    var config = new EHS_MediaSourceConfig(EHS_MediaSourceType.Video);
                    config.path = video;
                    config.sides = sides;
                    if(!mediaContinue){
                        source.Stop(video);
                    }
                    source.Detach(ehsObject, config);
                }
            }
            // @TODO - image if needed
            return true;
        }catch(Exception e){ 
            Debug.LogException(e);
        }
        return false;
    }

    public EHS_QuadHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        // (1) - Material handler
        objectDetailHandlers.Add(EHS_DetailType.Surface, new QuadSurfaceHandler(manager));
        // (2) ...
    }

    public class QuadSurfaceHandler : ObjectDetailHandler
    {
        public QuadSurfaceHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all material handlers
            // (1) image media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Image, new MediaSourceMaterialDetailHandler(manager));
            // (2) video media source
            objectDetailValuesHandlers.Add(EHS_MediaSourceType.Video, new MediaSourceMaterialDetailHandler(manager));
            // (3) text
            objectDetailValuesHandlers.Add(EHS_DetailType.Text, new TextDetailHandler(manager));
            // (4) background
            objectDetailValuesHandlers.Add(EHS_DetailType.View, new ColorDetailHandler(manager));
            // (5) clear
            objectDetailValuesHandlers.Add(EHS_DetailType.Clear, new ClearSurfaceDetailHandler(manager));
            // (6) fade
            objectDetailValuesHandlers.Add(EHS_DetailType.Fade, new FadeDetailHandler(manager));
        }

        public abstract class QuadSurfaceDetailValuesHandler : IObjectDetailValuesHandler
        {
            protected EHS_SceneManager manager;

            public QuadSurfaceDetailValuesHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            protected abstract bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs);

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                try{
                    ClearSurface(ehsObject);
                    return UpdateSurface(ehsObject, id, configs);
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }

            public bool Update(EHS_Object ehsObject, string [] values)
            { 
                return true;
            }
            private void ClearSurface(EHS_Object ehsObject)
            {
                try{
                    var quad = ehsObject.GetGameObject();
                    if(quad != null){
                        foreach(var textMesh in quad.GetComponentsInChildren<TextMesh>()){
                            if(textMesh != null){
                                textMesh.text = "";
                            }
                        }
                    }
                }catch(Exception e){
                    Debug.LogException(e);
                }
            }
        }

        public class MediaSourceMaterialDetailHandler : QuadSurfaceDetailValuesHandler
        {
            public MediaSourceMaterialDetailHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs)
            {
                string path = configs.GetStringConfig(EHS_Details.Path);
                EHS_MediaSource source = ehsObject.GetMediaSourceHandler().GetMediaSource(type);
                return source.Attach(ehsObject, EHS_MediaSourceConfig.Create(ehsObject.GetId(), type, path,
                                     QuadTextureHandler.HandlerType, null));
            }
        }

        public class TextDetailHandler : QuadSurfaceDetailValuesHandler
        {
            public TextDetailHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs)
            {
                string text = configs.GetStringConfig(EHS_Details.Text,"");
                var textConfig = configs.GetConfig(EHS_TextConfig.TypeName) as EHS_TextConfig; 
                if(textConfig != null){
                    float fontSize = 0.5f * textConfig.fontSize;
                    float fontScale = 0.4f * textConfig.fontScale;
                    string color = textConfig.color;
                    string textAlignment = textConfig.alignment;
                    string textStyle = textConfig.fontStyle;
                    try{
                        GameObject prefab = manager.GetPrefab(EHS_ObjectType.Text);
                        if(prefab == null){
                            return false;
                        }
                        GameObject quad = ehsObject.GetGameObject();
                        var mesh = quad.GetComponentInChildren<TextMesh>();
                        if(mesh == null){
                            GameObject textObject = GameObject.Instantiate(prefab, quad.transform);
                        // @TODO not using
                        //    mesh = EHS_TextHandler.GetTextMesh(quad, textObject, 1f/fontScale);
                        }
                        //EHS_TextHandler.UpdateFontSize(mesh, fontSize, 1f);
                        EHS_TextHandler.UpdateTextAlignment(mesh, textAlignment);
                        EHS_TextHandler.UpdateFontStyle(mesh, textStyle);
                        EHS_TextHandler.UpdateText(mesh, EHS_Utils.FixText(text));
                        if(!string.IsNullOrEmpty(text) && !string.IsNullOrEmpty(color)){ 
                            EHS_TextHandler.UpdateForegroundColor(ehsObject, mesh.gameObject, 
                                EHS_Utils.PareseColor(color, Color.white));
                        }
                        return true;
                    }catch(Exception e){
                        Debug.LogException(e);
                    }
                }
                return false;
            }
        }

        public class ColorDetailHandler : QuadSurfaceDetailValuesHandler
        {
            public ColorDetailHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs)
            {
                try{
                    string bgrColor = configs.GetStringConfig(EHS_Details.BgrColor,null);
                    string fgrColor = configs.GetStringConfig(EHS_Details.FgrColor,null);
                    GameObject quad = ehsObject.GetGameObject();
                    UpdateColor(ehsObject, quad, bgrColor, fgrColor, ehsObject.GetId());
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class ClearSurfaceDetailHandler : QuadSurfaceDetailValuesHandler
        {
            public ClearSurfaceDetailHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs)
            {
                try{
                    string surfaceObject = configs.GetStringConfig(EHS_Details.Object);
                    if(!string.IsNullOrEmpty(surfaceObject)){
                        if(surfaceObject.Equals("texture")){ 
                            ClearTexture(ehsObject, ehsObject.GetGameObject(), ehsObject.GetId());
                            return true;
                        }else{
                            if(surfaceObject.Contains("|text")){ 
                                EHS_TextHandler.ClearSurfaceText(ehsObject.GetGameObject());
                            }
                            return ClearMedia(ehsObject, false, surfaceObject, null);
                        }
                    }
                }catch(Exception e){ 
                    Debug.LogException(e);
                }
                return true;
            }
        }

        public class FadeDetailHandler : QuadSurfaceDetailValuesHandler
        {
            public FadeDetailHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateSurface(EHS_Object ehsObject, string type, EHS_ConfigList configs)
            {
                try{
                    string bgrColor = configs.GetStringConfig(EHS_Details.BgrColor);
                    string fgrColor = configs.GetStringConfig(EHS_Details.FgrColor);
                    string fadeDirection = configs.GetStringConfig("direction");
                    float fadeSpeed = configs.GetFloatConfig("fadeSpeed");
                    manager.StopCoroutine(Fading(ehsObject,  bgrColor, fgrColor, fadeSpeed, fadeDirection));
                    if(!string.IsNullOrEmpty(fadeDirection) && fadeSpeed > 0f){ 
                        manager.StartCoroutine(Fading(ehsObject,  bgrColor, fgrColor, fadeSpeed, fadeDirection));
                    }else{
                        UpdateColor(ehsObject, ehsObject.GetGameObject(), bgrColor, fgrColor, ehsObject.GetId());
                    }
                }catch(Exception e){ 
                    Debug.LogException(e);
                }
                return true;
            }

            IEnumerator Fading(EHS_Object ehsObject, string bgrColor, string fgrColor, float fadeSpeed, string direction) 
            {
                var updater = new FadingUpdater(ehsObject.GetId(), ehsObject, ehsObject.GetGameObject(), direction, bgrColor, fgrColor, fadeSpeed);
                while(!updater.IsUpdateCompleted()){
                    updater.Update();
                    yield return null;
                }
                ehsObject.UpdateState(EHS_DetailType.Fade + "_" + direction, true);
            }

            public class FadingUpdater {
                private EHS_Object ehsObject;
                private GameObject quadObject;
                private string id, bgrColor, fgrColor;
                private float fadeSpeed, bgrAlpha, fgrAlpha, alpha;
                private bool isBrgFadingDone, isFrgFadingDone, isFadeIn;
                private bool isUpdateCompleted = false;

                public FadingUpdater(string id, EHS_Object ehsObject, GameObject quadObject, string direction, string bgrColor, string fgrColor, float fadeSpeed){
                    this.id = id;
                    this.ehsObject = ehsObject;
                    this.quadObject = quadObject;
                    this.bgrColor = bgrColor;
                    this.fgrColor = fgrColor;
                    this.fadeSpeed = fadeSpeed;
                    isBrgFadingDone = string.IsNullOrEmpty(bgrColor);
                    isFrgFadingDone = string.IsNullOrEmpty(fgrColor);
                    isFadeIn = direction.Equals("in");
                    bgrAlpha = EHS_Utils.Alpha(bgrColor);
                    fgrAlpha = EHS_Utils.Alpha(fgrColor);
                    alpha = (isFadeIn) ? 0f : 1f; 
                }

                public void Update(){
                    if(isUpdateCompleted){
                        return;    
                    }
                    float delta = fadeSpeed * Time.deltaTime;
                    if(isFadeIn){
                        isBrgFadingDone = isBrgFadingDone || (bgrAlpha < alpha);
                        isFrgFadingDone = isFrgFadingDone || (fgrAlpha < alpha);
                        bgrColor = !isBrgFadingDone ? EHS_Utils.ChangeAlpha(bgrColor, alpha) : bgrColor;
                        fgrColor = !isFrgFadingDone ? EHS_Utils.ChangeAlpha(fgrColor, alpha) : fgrColor;
                        alpha += delta;
                    }else{
                        isBrgFadingDone = isBrgFadingDone || (0 > alpha);
                        isFrgFadingDone = isFrgFadingDone || (0 > alpha);
                        bgrColor = !isBrgFadingDone && bgrAlpha > alpha ? EHS_Utils.ChangeAlpha(bgrColor, alpha) : bgrColor;
                        fgrColor = !isFrgFadingDone && fgrAlpha > alpha ? EHS_Utils.ChangeAlpha(fgrColor, alpha) : fgrColor;
                        alpha -= delta;
                    }
                    if(!isBrgFadingDone || !isFrgFadingDone){
                        UpdateColor(ehsObject, quadObject, bgrColor, fgrColor, id);
                    }else{
                        if(isFadeIn){
                            bgrColor = EHS_Utils.ChangeAlpha(bgrColor, bgrAlpha);
                            fgrColor = EHS_Utils.ChangeAlpha(fgrColor, fgrAlpha);
                            UpdateColor(ehsObject, quadObject, bgrColor, fgrColor, id);
                        }else{
                            bgrColor = EHS_Utils.ChangeAlpha(bgrColor, 0f);
                            fgrColor = EHS_Utils.ChangeAlpha(fgrColor, 0f);
                            UpdateColor(ehsObject, quadObject, bgrColor, fgrColor, id);
                        }
                        isUpdateCompleted = true;
                    }
                }

                public bool IsUpdateCompleted(){ 
                    return isUpdateCompleted;        
                }
            } 
        }
    }

    public class QuadTextureHandler : TextureHandler
    {
        public static readonly string HandlerType = "quad";

        public QuadTextureHandler(EHS_MediaSource mediaSource) : base(mediaSource)
        {
        }

        public override bool Update(EHS_Object ehsObject, Texture texture, string[] sides)
        {
            try{
                ApplyImage(ehsObject, texture);
                return true;
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }
    }
}
