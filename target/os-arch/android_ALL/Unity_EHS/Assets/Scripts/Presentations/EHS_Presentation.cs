
using UnityEngine;

public abstract class EHS_Presentation : MonoBehaviour
{
    // special lock types
    public static readonly string CreateLock = "create";
    public static readonly string DestroyLock = "destroy";

    private bool locked = false;
    private bool isCreated = false;
    private bool isFirstExec = true;
    private string lockedType = null;
    private EHS_PresentationConfig config;
    private EHS_Queue<EHS_PresentationConfig> queue;
    protected EHS_SceneManager manager;
    protected EHS_PresentationObject presentationObject;

    public void Initialise(EHS_PresentationConfig config, EHS_SceneManager manager)
    {
        // create update
        if(!isCreated){
            this.manager = manager;
            this.config = config;
            presentationObject = CreatePresentationObject(config);
            OnCreate();
            queue = new EHS_Queue<EHS_PresentationConfig>();
            locked = false;
            lockedType = null;
            isCreated = true;
            isFirstExec = true;
        }
    }

    public void SetMedia(EHS_PresentationConfig config)
    {
        if(!isCreated){
            Debug.Log("Unable to set presention media, as it wasn't initialised.");
            return;
        }
        // set media update
        this.config.type = config.type;
        this.config.path = config.path;
        this.config.text = config.text;
        var temp = this.config.Copy();
        if(config.textConfig != null && config.textConfig.update){
            temp.textConfig = config.textConfig;
        }
        if(config.updateAnimationConfig != null && config.updateAnimationConfig.update){
            temp.updateAnimationConfig = config.updateAnimationConfig;
        }
        queue.Push(temp);
    }

    public virtual void StopMedia()
    {
        if(presentationObject != null){
            presentationObject.Stop();
        }
    }

    public virtual void OnPresentationDestroy(EHS_Object ehsObject)
    {
        if(presentationObject != null){
            presentationObject.Destroy();
        }
    }

    public void AddCommand(string command)
    {
        if(!manager.AddCommand(command)){
            Debug.Log("Failed to add command => " + command);
        }
    }

    public void AddCommand(EHS_CommandConfig command)
    {
        if(!manager.AddCommand(command)){
            Debug.Log("Failed to add command => " + command);
        }
    }

    // returns an main presentation config
    public EHS_PresentationConfig GetConfig(){ 
        return config;
    }

    public string GetId(){ 
        if(config != null){ 
            return config.id;    
        }
        return null;
    }

    public EHS_SceneManager GetSceneManager(){
        return manager;    
    }

    public void Lock(string type = null){
        if(IsSpecialLock(lockedType)){ 
            Debug.LogWarning("Unable to lock presentation ("+GetId()+"). Must be unlocked using '"+lockedType+" lock' first.");
            return;
        }
        lockedType = type;
        locked = true;
        //Debug.LogWarning("***** Locked ("+GetId()+") ******");
    }

    public void Unlock(string type = null){
        if(IsSpecialLock(lockedType) && !lockedType.Equals(type)){
            Debug.LogWarning("Unable to unlock presentation ("+GetId()+"). Must be unlocked using '"+lockedType+" lock' first.");
            return;
        }
        locked = false;
        lockedType = null;
        //Debug.LogWarning("***** Unlocked ("+GetId()+") ******");
    }

    public bool IsLocked(string type = null){
        if(!string.IsNullOrEmpty(lockedType) && locked){
            if(!IsSpecialLock(lockedType) && !lockedType.Contains(type)){ 
                return false;    
            }
        }
        return locked;
    }

    public bool IsSpecialLock(string type){ 
        return CreateLock.Equals(type) || DestroyLock.Equals(type);
    }

    protected virtual void InitExec(){ 
        if(presentationObject != null){
            presentationObject.InitObject();
        }
    }
    protected virtual void OnCreate(){
        if(presentationObject != null){
            presentationObject.Create(config);
        }    
    }

    protected virtual void Exec(EHS_PresentationConfig config){
        if(presentationObject != null){
            presentationObject.Update(config);
        }  
    }

    protected virtual EHS_PresentationObject CreatePresentationObject(EHS_PresentationConfig config){
        return null;  
    }

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if(queue.Ready() && !IsLocked(queue.GetFirst().type)){
            //Debug.LogWarning("Queue ("+config.type+") size: " + queue.Size());
            if(isFirstExec){ 
                InitExec();
                isFirstExec = false;    
            }
            Exec(queue.Grab());
        }
    }
}
