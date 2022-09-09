using System.Collections.Generic;

public abstract class EHS_ObjectUpdateAnimation : EHS_Animation
{
    public bool isAnimatingOnText { get; private set; }
    public string initalSide = EHS_ObjectUtils.BackSide;
    protected EHS_PresentationConfig config { get; private set; }
    protected Dictionary<string, ObjectMedia> objectMediaMap;

    public EHS_ObjectUpdateAnimation(EHS_Presentation presentation, string id, string objectType) : base (presentation, id, objectType) { 
        Configure(presentation.GetConfig());
    }

    public void Update(EHS_PresentationConfig config){ 
        this.config = config;
    }

    private void Configure(EHS_PresentationConfig config){ 
        if(config == null) return;
        // configure animation
        isAnimatingOnText = config.IsFlag(EHS_PresentationFlags.AnimateOnText);
    }

    public override void InitAnimation(EHS_PresentationObject presentationObject){ 
        this.presentationObject = presentationObject;
        EHS_AnimationConfig animationConfig = presentationObject.GetConfig().updateAnimationConfig;
        InitAnimation(animationConfig);
    }

    public virtual void PreAnimate(){ /* overwrite with stuff that needs to be done before animating */ }

    public virtual void OnAnimationReplace(EHS_AnimationConfig animationConfig){ /* overwrite with stuff that needs to be done after replacing animation */ }

    public virtual void PostStop(){
        if(config == null) return;
        if(IsApplyingMedia()){
            AddMedia(config, initalSide);
        }else if(IsTextAvailable()){ 
            AddText(config, initalSide);
        }
    }

    protected virtual bool HasValidMedia(string type, string path){ 
        return (EHS_MediaSource.IsSupportedMedia(type) && !string.IsNullOrEmpty(path));
    }

    protected virtual bool HasValidMedia(){ 
        return HasValidMedia(config.type, config.path);
    }

    protected virtual bool IsApplyingMedia(){ 
        return !config.IsTextOnly() && HasValidMedia();        
    }

    protected virtual bool IsTextAvailable(){ 
        return config.IsTextOnly() || (!config.IsTextOnly() && !string.IsNullOrEmpty(config.text));   
    }

    protected void AddMedia(EHS_PresentationConfig config, string type, string path, string side){ 
        // create media source
        AddCommand(EHS_MediaSource.CreateCommand(id, type, path));
        // attach media to the object
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(objectType);
        command.objectConfig.id = id;
        command.objectConfig.keep = true;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(type)
                            .appendConfig(EHS_Details.Path, path)
                            .appendConfig(EHS_Details.Side, side)
                            .appendConfig(EHS_Details.BgrColor, config.bgrColor)
                            .appendConfig(config.textConfig));
        AddCommand(command);
        ObjectMedia objectMedia = GetObjectMedia(side);
        if(objectMedia != null){
            objectMedia.Add(type, path);
        }
    }

    protected void AddMedia(EHS_PresentationConfig config, string side){ 
        AddMedia(config, config.type, config.path, side);
    }

    protected void AddText(EHS_PresentationConfig config, string text, string side){
        // attach text to the object
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(objectType);
        command.objectConfig.id = id;
        command.objectConfig.keep = true;
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(EHS_DetailType.Text)
                            .appendConfig(EHS_Details.Side, side)
                            .appendConfig(EHS_Details.Text, text)
                            .appendConfig(config.textConfig));
        AddCommand(command);
        ObjectMedia objectMedia = GetObjectMedia(side);
        if(objectMedia != null){
            objectMedia.Add(EHS_DetailType.Text, text);
        }
    }

    protected void AddText(EHS_PresentationConfig config, string side){ 
        AddText(config, config.text, side);
    }

    protected void Clear(string media, string side, bool continuePlaying=false, bool clearTexture=false){ 
        var command = new EHS_CommandConfig("updateObject");
        command.objectConfig = new EHS_ObjectConfig(objectType);
        command.objectConfig.id = id;
        command.objectConfig.keep = true;
        // clear media
        command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                            .setConfigID(EHS_DetailType.Clear)
                            .appendConfig(EHS_Details.Side, side)
                            .appendConfig(EHS_Details.Object, media)
                            .appendConfig(EHS_Details.MediaContinue, continuePlaying));
        // clear texture
        if(clearTexture){
            command.objectConfig.appendDetail(new EHS_DetailConfig(EHS_DetailType.Surface)
                                .setConfigID(EHS_DetailType.Clear)
                                .appendConfig(EHS_Details.Side, side)
                                .appendConfig(EHS_Details.Object,"texture"));
        }
        AddCommand(command);
        ObjectMedia objectMedia = GetObjectMedia(side);
        if(objectMedia != null){
            objectMedia.Clear(media);
        }
    }

    protected void Clear(ObjectMedia objectMedia, string side, bool continuePlaying=false, bool clearTexture=false){ 
        if(objectMedia != null){
            Clear(objectMedia.ToMediaString(), side, continuePlaying, clearTexture); 
        }
    }

    protected void Clear(){
        ObjectMedia objectMedia = GetObjectMedia(initalSide);
        Clear(objectMedia, initalSide);
    }

    protected void ClearText(string side){ 
        Clear("|text", side);
    }

    protected void ClearTexture(string side, bool continuePlaying=false){
        ObjectMedia objectMedia = GetObjectMedia(initalSide);
        Clear(objectMedia, side, continuePlaying, true);
    }

    protected ObjectMedia GetObjectMedia(string side){
        if(objectMediaMap != null){
            if(!string.IsNullOrEmpty(side) && objectMediaMap.ContainsKey(side)){
                return objectMediaMap[side];
            }
        }
        return null;
    }

    public abstract void Stop();

    // @TODO - improve how media gets clearead . replace x|x| string with a proper list
    protected class ObjectMedia
    {
        public string side { private set; get; }
        public string latestNonText { private set; get; }
        public Dictionary<string, string> media  { private set; get; }

        public ObjectMedia(string side){ 
            this.media = new Dictionary<string, string>();
            this.side = side;
        }

        public void Add(string media, string path){
            if(!EHS_DetailType.Text.Equals(media)){
                latestNonText = media;
            }
            if(this.media.ContainsKey(media)){
                this.media[media] = path;
                return;
            }
            this.media.Add(media, path);
        }

        public void Clear(string stringMedia=null){ 
            if(string.IsNullOrEmpty(stringMedia)){
                latestNonText = null;
                media.Clear();
            }else{ 
                var array = ToMediaArray(stringMedia);
                foreach(var media in array){ 
                    if(string.IsNullOrEmpty(media)){ 
                        continue;
                    }
                    if(!EHS_DetailType.Text.Equals(media) && 
                        media.Equals(latestNonText)){
                        latestNonText = null;
                    }
                    this.media.Remove(media);
                }
            }
        }

        public bool HasMedia(){ 
            return (media.Count > 0);    
        }

        public bool HasText(){ 
            return media.ContainsKey(EHS_DetailType.Text);    
        }

        public bool HasNonTextMedia(){ 
            return HasMedia() && !HasText();    
        }

        public string ToMediaString(){ 
            string mediaString = "";
            foreach(var mediaEntry in media){
                mediaString += "|";
                mediaString += (mediaEntry.Key + ","+ mediaEntry.Value); 
            } 
            return mediaString;
        }

        public static string[] ToMediaArray(string mediaString){ 
            string[] array = null;
            if(!string.IsNullOrEmpty(mediaString)){ 
                if(mediaString.Contains("|")){ 
                    try{ 
                        array = mediaString.Split('|');    
                    }catch{}    
                }else{
                    array = new string[1];
                    array[0] = mediaString;        
                }
            }
            return array;
        }
    }
}
