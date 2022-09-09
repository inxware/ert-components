using System;
using System.Collections;
using UnityEngine;

public class EHS_ViewHandler : EHS_ObjectHandler
{
    public static readonly string VideoControl = "video";

    public EHS_ViewHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        objectDetailHandlers.Add(EHS_DetailType.View, new ViewDetailHandler(manager));
    }

    public class ViewDetailHandler : ObjectDetailHandler
    {
        public static readonly string STOP_ID = "viewStop";

        public static bool  FADE_ENABLE = false;
        public static float FADE_IN_STEPS = 0.1f;
        public static float FADE_IN_DELAY = 0.1f;
        public static float FADE_OUT_STEPS = 0.1f;
        public static float FADE_OUT_DELAY = 0.1f;

        public ViewDetailHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all view handlers
            // (1) control video
            objectDetailValuesHandlers.Add(VideoControl, new ViewDetailVideoControlHandler(manager));
            // (2) background color
            objectDetailValuesHandlers.Add(EHS_DetailType.Color, new ViewDetailBackgroundColorHandler(manager));
            // (3) fading settings
            objectDetailValuesHandlers.Add(EHS_DetailType.Fade, new ViewDetailFadeHandler(manager));

        }

        public override bool Stop(EHS_Object ehsObject)
        {
            Debug.Log("Stopping Camera View.");
            Camera camera = manager.GetCameraView();
            var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
            manager.StopCoroutine(StopPlay(ehsObject, videoPlayer));
            manager.StartCoroutine(StopPlay(ehsObject, videoPlayer));
            return true;
        }

        IEnumerator StopPlay(EHS_Object ehsObject, UnityEngine.Video.VideoPlayer videoPlayer) 
        {
            // stop video
            if(videoPlayer.enabled){
                if(FADE_ENABLE){
                    // fade down video
                    for (float alpha = videoPlayer.targetCameraAlpha; alpha >= 0; alpha -= FADE_OUT_STEPS) 
                    {
                        videoPlayer.targetCameraAlpha = alpha;
                        yield return new WaitForSeconds(FADE_OUT_DELAY);
                    }
                }
                videoPlayer.Stop();
                videoPlayer.enabled = false;
                ehsObject.Stop(STOP_ID, true);
            }
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

        public class ViewDetailVideoControlHandler : ViewDetailValueHandler
        {
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
                    var videoPlayer = camera.GetComponentInParent<UnityEngine.Video.VideoPlayer>();
                    manager.StopCoroutine(FadingPlay(videoPlayer, path));
                    manager.StartCoroutine(FadingPlay(videoPlayer, path));
                    return true;
                }catch(System.Exception e){
                    Debug.LogException(e);
                }
                return false;
            }

            IEnumerator FadingPlay(UnityEngine.Video.VideoPlayer videoPlayer, string path) 
            {
                // stop video
                if(videoPlayer.enabled && videoPlayer.isPlaying){
                    // fade down video
                    if(FADE_ENABLE){
                        for (float alpha = videoPlayer.targetCameraAlpha; alpha >= 0f; alpha -= FADE_OUT_STEPS){
                            if(alpha < 0f){ 
                                alpha = 0f;    
                            }
                            videoPlayer.targetCameraAlpha = alpha;
                            yield return new WaitForSeconds(FADE_OUT_DELAY);
                        }
                    }
                    videoPlayer.Stop();
                }
                // start video
                videoPlayer.enabled = true;
                videoPlayer.url = path;
                videoPlayer.Prepare();
                int prepareCheckCount = 0;
                while(!videoPlayer.isPrepared){
                    if(prepareCheckCount >= 30){ 
                        break;    
                    }
                    prepareCheckCount++;
                    yield return new WaitForSeconds(.1f);
                }
                videoPlayer.Play();
                if(FADE_ENABLE){
                    // wait for the first frame
                    int checkCount = 0;
                    while(!videoPlayer.isPlaying || videoPlayer.frame <= 0){
                        if(checkCount >= 10){ 
                            break;
                        }
                        checkCount++;
                        yield return new WaitForSeconds(.2f);
                    }
                    for (float alpha = videoPlayer.targetCameraAlpha; alpha <= 1f; alpha += FADE_IN_STEPS){
                        videoPlayer.targetCameraAlpha = alpha;
                        yield return new WaitForSeconds(FADE_IN_DELAY);
                    }
                }
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
                    videoPlayer.targetCameraAlpha = EHS_Utils.Alpha(bgrColor);
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
                        FADE_IN_STEPS = configs.GetFloatConfig("fadeSteps");;
                        FADE_IN_DELAY = configs.GetFloatConfig("fadeDelay");
                    }else if(fading.Equals("out")){ 
                        FADE_ENABLE = true;
                        FADE_OUT_STEPS = configs.GetFloatConfig("fadeSteps");;
                        FADE_OUT_DELAY = configs.GetFloatConfig("fadeDelay");
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
    }
}
