using System;
using System.Collections;
using UnityEngine;

public class EHS_ViewHandler : EHS_ObjectHandler
{
    public static readonly string VideoControl = "video";
    public static readonly string ImageControl = "image";
    public static readonly string TextControl = "text";
    public static readonly string DisableAll = "disable_all";

    public EHS_ViewHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        objectDetailHandlers.Add(EHS_DetailType.View, new ViewDetailHandler(manager));
    }

    public class ViewDetailHandler : ObjectDetailHandler
    {
        public static readonly string STOP_ID = "viewStop";

        public static bool  FADE_ENABLE = false;
        public static float FADE_SPEED_IN = 1.0f;
        public static float FADE_SPEED_OUT = 1.0f;

        public static float START_DELAY = 1.0f;

        public ViewDetailHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all view handlers
            // (1) control video
            objectDetailValuesHandlers.Add(VideoControl, new ViewDetailVideoControlHandler(manager));
            // (2) background color
            objectDetailValuesHandlers.Add(EHS_DetailType.Color, new ViewDetailBackgroundColorHandler(manager));
            // (3) fading settings
            objectDetailValuesHandlers.Add(EHS_DetailType.Fade, new ViewDetailFadeHandler(manager));
            // (4) control image
            objectDetailValuesHandlers.Add(ImageControl, new ViewDetailImageControlHandler(manager));
            // (5) control text
            objectDetailValuesHandlers.Add(TextControl, new ViewDetailTextControlHandler(manager));
            // (6) clear
            objectDetailValuesHandlers.Add(DisableAll, new ViewDetailDisableHandler(manager));

        }

        public override bool Stop(EHS_Object ehsObject)
        {
            Debug.Log("Stopping Camera View.");
            Camera camera = manager.GetCameraView();
            GameObject textObject = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Text_TMP");
            if(textObject != null && textObject.activeSelf){
                var mesh = textObject.GetComponentInChildren<TMPro.TextMeshPro>();
                var handler = new EHS_TextMeshProHandler(mesh).Init(
                    new EHS_TextObjectConfig()
                            .SetEhsObject(ehsObject)
                            .SetParentObject(camera.gameObject)   
                );
                handler.Clear();
                textObject.SetActive(false);
            }
            if(FADE_ENABLE){
                manager.StopCoroutine(FadeStopPlay(ehsObject, camera));
                manager.StartCoroutine(FadeStopPlay(ehsObject, camera));
            }else {
                NormalStopPlay(ehsObject, camera);
            }
            
            return true;
        }

        static IEnumerator FadingStopVideo(UnityEngine.Video.VideoPlayer videoPlayer){ 
            // fade down video
            float delta = FADE_SPEED_OUT * Time.deltaTime;
            for (float alpha = videoPlayer.targetCameraAlpha; alpha >= 0.0f; alpha -= delta){
                videoPlayer.targetCameraAlpha = (alpha < 0.0f) ? 0.0f : alpha;
                yield return null;
            }
            videoPlayer.targetCameraAlpha = 0.0f;
            videoPlayer.Stop();
            videoPlayer.enabled = false;
        }
                
        static IEnumerator FadingStopImage(GameObject canvas){
            var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
            Color color = image.color;
            float delta = FADE_SPEED_OUT * Time.deltaTime;
            for (float alpha = color.a; alpha >= 0.0f; alpha -= delta){
                color.a = (alpha < 0.0f) ? 0.0f : alpha;
                image.color = color;
                yield return null;
            }
            color.a = 0.0f;
            image.color = color;
            image.texture = null;
            canvas.SetActive(false);
        }

        IEnumerator FadeStopPlay(EHS_Object ehsObject, Camera camera) 
        {
            var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
            // stop video
            if(videoPlayer.enabled){
                yield return FadingStopVideo(videoPlayer);
            }
            // stop fade-out image
            var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
            if(canvas.activeSelf){
                yield return FadingStopImage(canvas);
            }

            ehsObject.Stop(STOP_ID, true);
        }

        void NormalStopPlay(EHS_Object ehsObject,  Camera camera) 
        {
            var videoPlayers = camera.GetComponentsInParent<UnityEngine.Video.VideoPlayer>();
            foreach(var videoPlayer in videoPlayers){
                // stop video
                if(videoPlayer.enabled){
                    videoPlayer.Stop();
                    videoPlayer.targetCameraAlpha = 0.0f;
                    videoPlayer.enabled = false;
                }
            }

            var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
            if(canvas.activeSelf){
                var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
                Color color = image.color;
                color.a = 0.0f;
                image.color = color;
                image.texture = null;
                canvas.SetActive(false);
            }

            ehsObject.Stop(STOP_ID, true);
        }

        public abstract class ViewDetailValueHandler : IObjectDetailValuesHandler
        {
            protected EHS_SceneManager manager;

            public ViewDetailValueHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            protected abstract bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs);

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                try{ 
                    return UpdateView(ehsObject, configs);    
                }catch(Exception e){ 
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class ViewDetailTextControlHandler : ViewDetailValueHandler
        {
            public ViewDetailTextControlHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs) {

                try{
                    Camera camera = manager.GetCameraView();
                    GameObject textObject = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Text_TMP");
                    var mesh = textObject.GetComponentInChildren<TMPro.TextMeshPro>();
                    bool clear = configs.GetBoolConfig("Clear");
                    if(clear){
                        if(textObject.activeSelf){
                            var handler = new EHS_TextMeshProHandler(mesh).Init(
                                new EHS_TextObjectConfig()
                                        .SetEhsObject(ehsObject)
                                        .SetParentObject(camera.gameObject)   
                            );
                            handler.Clear();
                            //textObject.SetActive(false);
                        }
                    }else{
                        string text = configs.GetStringConfig(EHS_Details.Text, "");
                        var textConfig = configs.GetConfig(EHS_TextConfig.TypeName) as EHS_TextConfig;
                        if(textConfig != null && mesh != null){ 
                            var handler = new EHS_TextMeshProHandler(mesh).Init(
                                new EHS_TextObjectConfig()
                                        .SetEhsObject(ehsObject)
                                        .SetParentObject(camera.gameObject)   
                            );
                            textObject.SetActive(true);
                            handler.Update(textConfig, text);
                        }
                    }
                }catch(System.Exception e){
                    Debug.LogException(e);
                }
                return true;
            }
        }

        public class ViewDetailImageControlHandler : ViewDetailValueHandler
        {
            public ViewDetailImageControlHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs) {
                try{
                    Debug.Log("Starting Camera Image Background View.");
                    string path = configs.GetStringConfig(EHS_Details.Path);
                    if(string.IsNullOrEmpty(path)){
                        Debug.LogWarning("The camera view image path ("+path+") is invalid.");
                        return false;
                    }
                    
                    EHS_MediaSource source = ehsObject.GetMediaSourceHandler().GetMediaSource(EHS_MediaSourceType.Image);
                    return source.Attach(ehsObject, EHS_MediaSourceConfig.Create(ehsObject.GetId(), EHS_MediaSourceType.Image, path, 
                                         BackgroundImageTextureHandler.HandlerType, "all") );
                }catch(System.Exception e){
                    Debug.LogException(e);
                }
                return false;
            }

            public class BackgroundImageTextureHandler : TextureHandler
            {
                public static readonly string HandlerType = "backgroundimage"; 
                public BackgroundImageTextureHandler(EHS_MediaSource mediaSource) : base(mediaSource)
                {
                }

                static IEnumerator FadingStartImage(GameObject canvas, Texture texture){
                    var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
                    image.texture = texture;
                    Color color = image.color;
                    color.a = 0.0f;
                    image.color = color;
                    canvas.SetActive(true);
                    float delta = FADE_SPEED_IN * Time.deltaTime;
                    for (float alpha = color.a; alpha <= 1.0f; alpha += delta){
                        color.a = (alpha < 0.0f) ? 0.0f : alpha;
                        image.color = color;
                        yield return null;
                    }
                    color.a = 1.0f;
                    image.color = color;
                }

                IEnumerator UpdateFadingImage(Camera camera, Texture texture){ 
                    // stop fade-out video
                    var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
                    if(videoPlayer.enabled){
                        yield return FadingStopVideo(videoPlayer);
                    }
                    // stop fade-out image
                    var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
                    if(canvas.activeSelf){
                        yield return FadingStopImage(canvas);
                    }

                    yield return FadingStartImage(canvas, texture);
                }

                void UpdateImage(Camera camera, Texture texture){ 
                    // update image texture
                    var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
                    var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
                    Color color = image.color;
                    color.a = 1.0f;
                    image.color = color;
                    image.texture = texture;
                    canvas.SetActive(true);
                    // stop video players
                    var videoPlayers = camera.GetComponentsInParent<UnityEngine.Video.VideoPlayer>();
                    foreach(var videoPlayer in videoPlayers){
                        if(videoPlayer.isPlaying){ 
                            videoPlayer.Stop();
                        }
                        videoPlayer.enabled = false;
                    }
                }

                public override bool Update(EHS_Object ehsObject, Texture texture, string[] sides)
                {
                    try{
                        var manager = ehsObject.GetManager();
                        Camera camera = manager.GetCameraView();
                        if(FADE_ENABLE){ 
                            manager.StopCoroutine(UpdateFadingImage(camera, texture));
                            manager.StartCoroutine(UpdateFadingImage(camera, texture));
                        }else{ 
                            UpdateImage(camera, texture);
                        }
                        return true;
                    }catch(Exception e){
                        Debug.LogException(e);
                    }
                    return false;
                }
            }
        }

        public class ViewDetailVideoControlHandler : ViewDetailValueHandler
        {
            public static EHS_Timeout SyncTimeout = new EHS_Timeout();
            public ViewDetailVideoControlHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs){
                try{ 
                    Debug.Log("Starting Camera View.");
                    string path = configs.GetStringConfig(EHS_Details.Path);
                    if(string.IsNullOrEmpty(path) || !EHS_VideoSource.IsSupportedFile(path)){
                        Debug.LogWarning("The camera view video path ("+path+") is invalid.");
                        return false;
                    }
                    Camera camera = manager.GetCameraView();
                    
                    if(FADE_ENABLE){
                        var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
                        manager.StopCoroutine(PlayFading(videoPlayer, path));
                        manager.StartCoroutine(PlayFading(videoPlayer, path));
                    }else{
                        var videoPlayers = camera.GetComponentsInParent<UnityEngine.Video.VideoPlayer>();
                        PlayNormal(videoPlayers, path);
                    }
                    return true;
                }catch(System.Exception e){
                    Debug.LogException(e);
                }
                return false;
            }

            // ==================== Play with Fading Effect ========================

            IEnumerator FadingStartVideo(UnityEngine.Video.VideoPlayer videoPlayer){
                // wait for the first frame
                int checkCount = 0;
                while(!videoPlayer.isPlaying || videoPlayer.frame <= 0){
                    if(checkCount >= 20){ 
                        break;
                    }
                    checkCount++;
                    yield return new WaitForSeconds(0.1f);
                }
                float delta = FADE_SPEED_IN * Time.deltaTime;
                for (float alpha = videoPlayer.targetCameraAlpha; alpha <= 1.0f; alpha += delta){
                    videoPlayer.targetCameraAlpha = alpha;
                    yield return null;
                }
                videoPlayer.targetCameraAlpha = 1.0f;
            }

            void PreparedFading(UnityEngine.Video.VideoPlayer videoPlayer) {
                videoPlayer.Play();
                manager.StopCoroutine(FadingStartVideo(videoPlayer));
                manager.StartCoroutine(FadingStartVideo(videoPlayer));
            }

            IEnumerator PlayFading(UnityEngine.Video.VideoPlayer videoPlayer, string path) 
            {
                // stop fade-out video
                if(videoPlayer.enabled){
                    yield return FadingStopVideo(videoPlayer);
                }
                // stop fade-out image
                var canvas = EHS_ObjectUtils.GetChildWithName(videoPlayer.GetComponentInParent<Camera>().gameObject, "Canvas");
                if(canvas.activeSelf){
                    yield return FadingStopImage(canvas);
                }
                videoPlayer.enabled = true;
                videoPlayer.url = path;
                
                videoPlayer.prepareCompleted += PreparedFading;
                videoPlayer.Prepare();
            }

            // ==================== Play Normal (no fading effect) ========================
            void PlayNormalFrameReady(UnityEngine.Video.VideoPlayer videoPlayer, long frameIdx) { 

                videoPlayer.sendFrameReadyEvents = false;
                videoPlayer.frameReady -= PlayNormalFrameReady;

                var camera = videoPlayer.GetComponentInParent<Camera>();

                // remove image canvas
                var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
                var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
                image.texture = null;
                canvas.SetActive(false);

                // swap video players
                var videoPlayers = camera.GetComponentsInParent<UnityEngine.Video.VideoPlayer>();
                foreach(var otherPlayer in videoPlayers){ 
                    if(otherPlayer != videoPlayer){
                        otherPlayer.Stop();
                        otherPlayer.enabled = false;
                        break;
                    }
                }
                videoPlayer.enabled = true;
            }

            IEnumerator PlayNormalStart(UnityEngine.Video.VideoPlayer videoPlayer){
                videoPlayer.targetCameraAlpha = 1.0f;
                videoPlayer.sendFrameReadyEvents = true;
                videoPlayer.frameReady += PlayNormalFrameReady;
                // wait for the sync timeout to expire
                while(SyncTimeout.IsStrated() && !SyncTimeout.Timeout()){ 
                    yield return null; 
                }
                SyncTimeout.Stop();
                videoPlayer.Play();
            }

            void PreparedNormal(UnityEngine.Video.VideoPlayer videoPlayer) {
                videoPlayer.prepareCompleted -= PreparedNormal;

                manager.StopCoroutine(PlayNormalStart(videoPlayer));
                manager.StartCoroutine(PlayNormalStart(videoPlayer));
            }

            void PlayNormal(UnityEngine.Video.VideoPlayer[] videoPlayers, string path){
                // start sync timeout for video
                //SyncTimeout.SetTimeout(START_DELAY);
                //SyncTimeout.Start();

                UnityEngine.Video.VideoPlayer videoPlayer = null;
                // get player which is not enabled
                if(!videoPlayers[0].enabled){ 
                    videoPlayer = videoPlayers[0];
                }else {
                    videoPlayer = videoPlayers[1];
                }
                videoPlayer.enabled = true;
                videoPlayer.url = path;
                videoPlayer.prepareCompleted += PreparedNormal;
                videoPlayer.Prepare();
            }
        }

        public class ViewDetailBackgroundColorHandler : ViewDetailValueHandler
        {
            public ViewDetailBackgroundColorHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs){
                try{
                    string bgrColor = configs.GetStringConfig(EHS_Details.BgrColor,null);
                    Camera camera = manager.GetCameraView();
                    camera.backgroundColor = EHS_Utils.PareseColor(bgrColor, Color.black);
                    var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
                    float alpha = camera.backgroundColor.a;
                    videoPlayer.targetCameraAlpha = alpha;
                    var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
                    var image = canvas.GetComponent<UnityEngine.UI.RawImage>();
                    Color color = image.color;
                    color.a = alpha;
                    image.color = color;
                    return true;
                }catch(System.Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class ViewDetailFadeHandler : ViewDetailValueHandler
        {
            public ViewDetailFadeHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs){
                try{
                    string fading = configs.GetStringConfig("direction");
                    if(fading.Equals("in")){
                        FADE_ENABLE = true;
                        FADE_SPEED_IN = configs.GetFloatConfig("fadeSpeed");;
                    }else if(fading.Equals("out")){ 
                        FADE_ENABLE = true;
                        FADE_SPEED_OUT = configs.GetFloatConfig("fadeSpeed");;
                    }else if(fading.Equals("disable")){ 
                        FADE_ENABLE = false;
                    }else{ 
                        return false;    
                    }
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class ViewDetailDisableHandler : ViewDetailValueHandler
        {
            public ViewDetailDisableHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateView(EHS_Object ehsObject, EHS_ConfigList configs){
                // disable video player
                Camera camera = manager.GetCameraView();
                var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
                videoPlayer.enabled = false;
                // disable image
                var canvas = EHS_ObjectUtils.GetChildWithName(camera.gameObject, "Canvas");
                canvas.SetActive(false);
                // disable text
                //@todo
                return true;
            }
        }
    }
}
