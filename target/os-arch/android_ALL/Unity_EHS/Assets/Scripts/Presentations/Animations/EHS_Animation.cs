using System.Collections.Generic;

public abstract class EHS_Animation
{
    public interface ICompleteListener {
        void OnComplete();    
    }

    private List<ICompleteListener> completeListeners; 

    protected EHS_Presentation presentation;
    protected EHS_PresentationObject presentationObject;
    protected string id;
    protected string objectType;

    public EHS_Animation(EHS_Presentation presentation, string id, string objectType){
        this.presentation = presentation;
        this.id = id;
        this.objectType = objectType;
        this.completeListeners = new List<ICompleteListener>();
    }

    public abstract string AnimationType();

    public abstract void InitAnimation(EHS_PresentationObject presentationObject);

    public abstract void InitAnimation(EHS_AnimationConfig animationConfig);

    public abstract void Animate();

    public abstract void OnObjectUpdated(string type, EHS_Object ehsObject);

    public void SetPresentationObject(EHS_PresentationObject presentationObject){ 
        this.presentationObject = presentationObject;
    }

    public void AddCommand(EHS_CommandConfig command){ 
        presentation.AddCommand(command);    
    }

    public void AddCommand(string command){ 
        presentation.AddCommand(command);    
    }

    public void AddCompleteListener(ICompleteListener listener){
        if(listener != null){
            completeListeners.Add(listener);
        }
    }

    public void ClearCompleteListener(){
        completeListeners.Clear();    
    }

    public void CompleteAnimation(){
        // run complete listeners
        foreach(var listener in completeListeners){ 
            listener.OnComplete();    
        }
    }
}
