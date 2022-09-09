using System.Collections.Generic;
using UnityEngine;

public class EHS_RotatingAnimation : EHS_ObjectUpdateAnimation
{
    public static readonly string Name = "rotate";

    public class AngleSidePair {
        public string side;
        public float angle;
        public AngleSidePair(string side, float angle){
            this.side = side;
            this.angle = angle;
        }

        public AngleSidePair Copy(){ 
            return new AngleSidePair(side, angle);
        }
    }

    private static readonly List<AngleSidePair> AngleSide_HQuad = new List<AngleSidePair>
    {
        new AngleSidePair("front", 180f),
        new AngleSidePair("back", 360f)
    };

    private static readonly List<AngleSidePair> AngleSide_HCube = new List<AngleSidePair>
    {
        new AngleSidePair("left", 90f),
        new AngleSidePair("front", 180f),
        new AngleSidePair("right", 270f),
        new AngleSidePair("back", 360f)
    };

    public int numberOfSides { get; private set; }
    public int sideIndex { get; private set; }
    private string previousSide;
    private readonly List<AngleSidePair> angleSides;
    private AngleSidePair angleSide;
    private RotateBehaviourListener callback;
    protected bool isMediaReused = true;
    protected float steps = 0.1f;
    protected float speed = 5f;
    protected float targetError = 0.01f;

    public EHS_RotatingAnimation(EHS_Presentation presentation, string id, string objectType) 
        : base(presentation, id, objectType) {
        if(EHS_ObjectType.QuadCube.Equals(objectType)){
            this.angleSides = AngleSide_HCube;
            this.numberOfSides = 4;
        }else{ 
            this.angleSides = AngleSide_HQuad;
            this.numberOfSides = 2;
        }
        this.previousSide = "back";
        this.angleSide = new AngleSidePair("back", 0f);
        this.sideIndex = 0;
    }

    public override string AnimationType() {
        return Name;
    }

    protected override bool IsTextAvailable(){ 
        return !string.IsNullOrEmpty(config.text);   
    }

    public override void InitAnimation(EHS_AnimationConfig animationConfig) {
        objectMediaMap = EHS_ObjectUtils.CreateSideDictionary<ObjectMedia>(null);
        foreach(var side in EHS_ObjectUtils.AllSidesArray){ 
            objectMediaMap[side] = new ObjectMedia(side);
        }
        if(animationConfig != null){ 
            steps = (animationConfig.steps > 0f) ? animationConfig.steps : steps;
            speed = (animationConfig.speed > 0f) ? animationConfig.speed : speed;
        }
    }

    public override void OnObjectUpdated(string type, EHS_Object ehsObject) {
        // do nothing
    }

    public override void Stop() {
        if(previousSide != null){
            angleSide = GetAngleSidePair(previousSide);
        }
    }

    public override void PreAnimate() {
        ApplyMedia(angleSide.side, previousSide);
        angleSide.angle = angleSides[sideIndex].angle;
        angleSide.side = angleSides[sideIndex].side;
    }

    public override void Animate(){
        bool isRotating = ApplyMedia(angleSide.side, previousSide);
        if(isRotating){ 
            Rotate();
        }
        angleSide.angle = angleSides[sideIndex].angle;
        angleSide.side = angleSides[sideIndex].side;
    }

    protected bool ApplyMedia(string currentSide, string previousSide){ 
        bool isRotating = false;
        // is non-text media
        if(IsApplyingMedia()){
            AddMedia(config, currentSide);
            isMediaReused = false;
            isRotating = true;
        }
        // is text media
        if(IsTextAvailable()){
            string side = config.IsTextOnly() ? previousSide : currentSide;
            // rotate cube and re-use previous media when animating on text
            if(isAnimatingOnText && config.IsTextOnly()){ 
                try{
                    side = currentSide;
                    ObjectMedia objectMedia = GetObjectMedia(previousSide);
                    var type = objectMedia.latestNonText;
                    if(!string.IsNullOrEmpty(type)){
                        var path = objectMedia.media[type];
                        if(HasValidMedia(type, path)){
                            AddMedia(config, type, path, side);
                            isMediaReused = true;
                        }
                    }
                }catch(System.Exception e){ 
                    Debug.LogException(e);
                }
                isRotating = true;
            }
            AddText(config, side);
        }
        return isRotating;
    }

    private void Rotate(){ 
        var command = new EHS_CommandConfig("attachEvent");
        command.eventConfig = new EHS_EventConfig(EHS_RotateBy.TypeName);
        command.eventConfig.id = id;
        command.eventConfig.speed = speed;
        command.eventConfig.steps = steps;
        command.eventConfig.targetError = targetError;
        command.eventConfig.direction = new Vector3(0f, angleSide.angle, 0f);
        if(callback == null){
            var ehsObject = presentation.GetSceneManager().GetObject(id);
            if(ehsObject != null){
                callback = new RotateBehaviourListener(this);
                ehsObject.SetBehaviourCallBack(EHS_RotateBy.TypeName, callback);
                command.eventConfig.notifyOnComplete = true;
            }
        }
        // lock presentation events
        if(isAnimatingOnText){
            presentation.Lock("video|img|text");
        }else{
            presentation.Lock("video|img"); 
        }
        AddCommand(command);
        NextSide();
    }

    private void NextSide(){ 
        previousSide = angleSide.side;
        sideIndex++;
        if(sideIndex >= numberOfSides){
            sideIndex = 0;
        }
    }

    private AngleSidePair GetAngleSidePair(string side){
        if(!string.IsNullOrEmpty(side)){
            foreach(var pair in angleSides){
                if(side.Equals(pair.side)){
                    return pair.Copy();
                }    
            }
        }
        return null;
    }

    private class RotateBehaviourListener : IBehaviourListener {

        private EHS_RotatingAnimation animationObject;

        public RotateBehaviourListener(EHS_RotatingAnimation animationObject){
            this.animationObject = animationObject;
        }
        
        public void OnCompleted(){
            // clear surface
            ClearSurface();
            animationObject.CompleteAnimation();
            // unlock the cube events
            var presentation = animationObject.presentation;
            presentation.Unlock(); 
        }

        private void ClearSurface(){
            // send command to clear the surface
            int index = ClearIndex();
            var side = animationObject.angleSides[index].side;
            var objectMedia = animationObject.GetObjectMedia(side);
            if(objectMedia == null){ 
                return;
            }
            bool continuePlaying = animationObject.isMediaReused;
            animationObject.Clear(objectMedia, side, continuePlaying, true);
        }

        private int ClearIndex(){ 
            int index;

            if(animationObject.sideIndex == 0){
                index = animationObject.numberOfSides - 2;
            }else if(animationObject.sideIndex == 1){
                index = animationObject.numberOfSides - 1;
            }else{
                index = animationObject.sideIndex - 2;
            }
            return index;
        }
    }
}
