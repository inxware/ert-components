using System.Threading;
using UnityEngine;

public class EHS_ExternalMediaSource 
{
    private enum ExternalMediaState{ 
        None,
        Requsted,
        Running
    }

    public static readonly string Type = "mediaPlayer";
    private static readonly string ExternalFunction = "ExternalUnity_";

    private static EHS_ExternalMediaSource instance;
    private EHS_SafeQueue<EHS_FunctionBlockConfig> queue;
    private EHS_Runnable runnable;
    private ExternalMediaState mediaState = ExternalMediaState.None;
    private string id;


    public static EHS_ExternalMediaSource GetInstance(){ 
        if(instance == null){ 
            instance = new EHS_ExternalMediaSource();    
        }
        return instance;
    }

    private EHS_ExternalMediaSource(){
        queue = new EHS_SafeQueue<EHS_FunctionBlockConfig>();
        runnable = new ExternalMediaPlayerRunnable(this);
    }

    public void Start(){ 
        try{
            runnable.Start();
        }catch(System.Exception e){ 
            Debug.LogException(e);    
        }
    }

    public void Stop(){ 
        try{
            runnable.Stop();
        }catch(System.Exception e){ 
            Debug.LogException(e);    
        }
    }

    public bool IsActive(){ 
        return !GetMediaState().Equals(ExternalMediaState.None);
    }

    public bool IsExternal(EHS_FunctionBlockConfig config){
        try{
            return config.cmd.Equals(EHS_FunctionBlockCommand.CREATE_COMMAND) &&
                   config.parameters.type.Equals(Type);
        }catch(System.Exception e){
            Debug.LogException(e);
        }
        return false;
    }

    public bool Process(EHS_FunctionBlockConfig config){
        if(config.cmd.Equals(EHS_FunctionBlockCommand.CREATE_COMMAND)){
            Debug.Log("Updating media state ["+mediaState.ToString()+" ==> None]");
            string id = GetCurrentId();
            if(!string.IsNullOrEmpty(id)){
                RequestDestroy(id);
            }
            if(!IsExternal(config)){
                return true;
            }
        }else if(config.cmd.Equals(EHS_FunctionBlockCommand.DESTROY_COMMAND)){ 
            RequestDestroy(GetCurrentId());
            return false;
        }

        if(GetMediaState().Equals(ExternalMediaState.None)){
            Debug.Log("Updating media state [None ==> Requsted]");
            SetMediaState(ExternalMediaState.Requsted);    
        }
        queue.Push(config);
        runnable.Notify();
        return false; // don't add command to the unity main thread queue
    }

    private void RequestDestroy(string id){ 
        Debug.Log("External media request [Destroy]");
        queue.Push(EHS_FunctionBlockConfig.Create(id, EHS_FunctionBlockCommand.DESTROY_COMMAND));
        runnable.Notify();
        EHS_Utils.Wait(5000, delegate(){ return GetMediaState().Equals(ExternalMediaState.None); });
    }

    private void SetCurrentId(string id){
        lock(this){
            this.id = id;
        }
    }

    private string GetCurrentId(){
        lock(this){
            return id;
        }
    }

    private void SetMediaState(ExternalMediaState mediaState){
        lock(this){
            this.mediaState = mediaState;
        }
    }

    private ExternalMediaState GetMediaState(){
        lock(this){
            return mediaState;
        }
    }

    private class ExternalMediaPlayerRunnable : EHS_Runnable { 
        
        private EHS_ExternalMediaSource mediaSource;
        private ExternalMediaPlayer mediaPlayer;

        public ExternalMediaPlayerRunnable(EHS_ExternalMediaSource mediaSource){ 
            this.mediaSource = mediaSource;
            this.mediaPlayer = new ExternalMediaPlayer();
        }

#if UNITY_ANDROID
        protected override void OnRunStart(){
            AndroidJNI.AttachCurrentThread();
        }

        protected override void OnRunStop(){
            AndroidJNI.DetachCurrentThread();
        }
#endif
        protected override void Run() {
            try{
                Debug.Log("External media runnable state [LOCKED]");
                Lock(); // locked until notified
                Debug.Log("External media runnable state [UNLOCKED]");
                var queue = mediaSource.queue;
                var mediaState = mediaSource.GetMediaState();
                while(queue.Ready()){
                    var config = queue.Grab();
                    if(config.cmd.Equals(EHS_FunctionBlockCommand.CREATE_COMMAND)){
                        mediaSource.SetCurrentId(config.id);
                    }else if(config.cmd.Equals(EHS_FunctionBlockCommand.DESTROY_COMMAND)){
                        //Debug.Log("Updating media state ["+mediaState.ToString()+" ==> Destroying]");
                        //mediaSource.SetMediaState(ExternalMediaState.Destroying);
                        mediaPlayer.Destroy(config.id);
                        if(!mediaPlayer.WaitUnavailable(config.id, 5000)){
                            Debug.LogWarning("Failed to destroy External Media ("+config.id+")");
                        }
                        Debug.Log("Updating media state ["+mediaState.ToString()+" ==> None]");
                        mediaSource.SetCurrentId(null);
                        mediaSource.SetMediaState(ExternalMediaState.None);
                    }else if(config.cmd.Equals(EHS_FunctionBlockCommand.SET_MEDIA_COMMAND)){ 
                        if(EHS_MediaSourceType.Video.Equals(config.type)){
                            mediaPlayer.Play(config.id, config.path);
                            if(mediaPlayer.WaitAvailable(config.id, 5000)){
                                if(mediaState.Equals(ExternalMediaState.Requsted)){
                                    Debug.Log("Updating media state [Requsted ==> Running]");
                                    mediaSource.SetMediaState(ExternalMediaState.Running);  
                                }
                            }
                        }else{ 
                            Debug.LogWarning("The external media doesn't support this type:"+config.type+".");
                        }
                    }else if(config.cmd.Equals(EHS_FunctionBlockCommand.STOP_MEDIA_COMMAND)){ 
                        mediaPlayer.Stop(config.id);
                    }else{
                        Debug.LogWarning("Unknown External Media cammand:"+config.cmd);      
                    }
                }
                // @TODO - handle manual return
            }catch(System.Exception e){ 
                Debug.LogException(e);    
            }
            //EHS_Runnable.Sleep(Delay);
        }
    }

    private class ExternalMediaPlayer {

        private static readonly string MediaLoad = "load";
        private static readonly string MediaStop = "stop";
        private static readonly string MediaDestroy = "destroy";
        private static readonly string ExternalMediaSourceFunction = ExternalFunction + "MediaSource";
        private static readonly string ExternalMediaSourceAvailableFunction = ExternalFunction + "IsMediaSourceAvailable";

        public ExternalMediaPlayer(){ 
 
        }
        
        public bool IsAvailable(string id){ 
            return EHS_ExternalUtils.ReadExternal<bool>(ExternalMediaSourceAvailableFunction, id);
        }

        public bool WaitAvailable(string id, int timeout){
            return EHS_Utils.Wait(timeout, delegate(){ return IsAvailable(id); });
        }

        public bool WaitUnavailable(string id, int timeout){
            return EHS_Utils.Wait(timeout, delegate(){ return !IsAvailable(id); });
        }

        public void Play(string id, string path){ 
            var external = ExternalMediaData.Create()
                .Append("type", "video")
                .Append("path", path)
                .Append("looping", true);
            EHS_ExternalUtils.WriteExternal(ExternalMediaSourceFunction, id, MediaLoad, external.data);
        }

        public void Stop(string id){ 
            EHS_ExternalUtils.WriteExternal(ExternalMediaSourceFunction, id, MediaStop);
        }

        public void Destroy(string id){ 
            EHS_ExternalUtils.WriteExternal(ExternalMediaSourceFunction, id, MediaDestroy);
        }
    }

    private class ExternalMediaData{

        private static readonly string DATA_SPLIT = ",";
        private static readonly string DATA_ENTRY = ":=";

        public string data { get; private set; }  

        public static ExternalMediaData Create(){ 
            return new ExternalMediaData();    
        }
        
        public ExternalMediaData Append(string key, string value){ 
            if(!string.IsNullOrEmpty(data)){ 
                data += (DATA_SPLIT + key + DATA_ENTRY + value);
            }else{
                data = (key + DATA_ENTRY + value);
            }
            return this;
        }

        public ExternalMediaData Append<T>(string key, T value){ 
            return Append(key, value.ToString());
        }
    }
}