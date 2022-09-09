
using System.Collections.Generic;
using UnityEngine;

public class EHS_FunctionBlockManager : MonoBehaviour
{
    // contains live ehs commands
    private EHS_SafeQueue<ICommand> commands;
    // scene manager
    public EHS_SceneManager manager;
    // stores pointers to all succesfully created ehs function block objects
    private Dictionary<string/*id*/, EHS_FBObject/*object*/> ehsFbObjects;
    // function block handler
    private EHS_FunctionBlockHandler functionBlockHandler;
    // queue used for caching the commands coming from EHS
    private FunctionBlockQueue functionBlockQueue;
    // contains live unity response commands
    private EHS_SafeQueue<EHS_FBResponse> responseQueue;
    // this flag is set to true to prevent processing next command.
    private bool locked = false;

    // Start is called before the first frame update
    void Start()
    {
        commands = new EHS_SafeQueue<ICommand>();
        ehsFbObjects = new Dictionary<string, EHS_FBObject>();
        functionBlockHandler = new EHS_FunctionBlockHandler(this);
        responseQueue = new EHS_SafeQueue<EHS_FBResponse>();
        functionBlockQueue = new FunctionBlockQueue(this);
        functionBlockQueue.Start();
        locked = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(!IsLocked() && !manager.IsLocked()){
            if(commands.Ready()){
                ICommand command = commands.Grab();
                command.Exec();
            }
        }
    }

    void OnDisable()
    { 
        if(functionBlockQueue != null){ 
            functionBlockQueue.Stop();    
        }
    }

    public bool IsLocked(){ 
        return locked;    
    }

    public void Lock(){
        Debug.Log("[LOCK] EHS Function Block Manager");
        locked = true;
    }

    public void Unlock(){
        Debug.Log("[UNLOCK] EHS Function Block Manager");
        locked = false;
    }

    //
    // Command Manager functions
    //
    public void AddCommand(EHS_FunctionBlockConfig config){
        functionBlockQueue.Add(config);
    }

    public void AddCommand(string jsonCommand){
        EHS_FunctionBlockConfig config = EHS_FunctionBlockConfig.CreateFromJSON(jsonCommand);
        AddCommand(config);
    }

    private void ProcessCommand(EHS_FunctionBlockConfig config){ 
        if(config != null){
            ICommand command = new EHS_FunctionBlockCommand(this, manager, config);
            if(command != null){
                // pre-process command.
                if(!PreProcessCommand(config)){
                    // do not continue if false
                    return;
                }
                // add command to the que
                commands.Push(command);
                // apply any essential post-process commands
                List<ICommand> postCommnads = GetPostProcessCommands(config);
                foreach(var postCommand in postCommnads){ 
                    commands.Push(postCommand);
                }
            }
        }
    }

    private class FunctionBlockQueue : EHS_Runnable { 
        
        private EHS_FunctionBlockManager manager;
        private EHS_SafeQueue<EHS_FunctionBlockConfig> queue;

        public FunctionBlockQueue(EHS_FunctionBlockManager manager){ 
            this.manager = manager;
            this.queue = new EHS_SafeQueue<EHS_FunctionBlockConfig>();
        }

        public void Add(EHS_FunctionBlockConfig config){ 
            queue.Push(config);
            Notify();
        }

        protected override void Run() { 
            Lock();
            while(queue.Ready()){ 
                manager.ProcessCommand(queue.Grab());
            }
        }
    }

    public EHS_SafeQueue<EHS_FBResponse> GetResponseQueue()
    {
        return responseQueue;
    }

    //
    // EHS function block handling functions
    //
    public EHS_FBObject CreateObject(EHS_FunctionBlockConfig config)
    {
        if(config == null){
            Debug.LogWarning("Attempting to create EHS Function Block object witth null config.");
            return null;
        }
        string id = config.id;
        if(id != null && id.Length > 0){
            if(!ehsFbObjects.ContainsKey(id)){
                EHS_FBObject ehsFbObject = new EHS_FBObject(config);
                if(functionBlockHandler.CreateObject(ehsFbObject)){
                    ehsFbObjects.Add(id, ehsFbObject);
                    return ehsFbObject;
                }else{
                    Debug.Log("Failed to create new EHS FB object (" + config.id + ").");
                }
            }else{
                // re-create the block
                ehsFbObjects.Remove(id);
                EHS_FBObject ehsFbObject = new EHS_FBObject(config, true);
                if(functionBlockHandler.CreateObject(ehsFbObject)){
                    ehsFbObjects.Add(id, ehsFbObject);
                    return ehsFbObject;
                }else{
                    Debug.Log("Failed to re-create new EHS FB object (" + config.id + ").");
                }
            }
        }else{
            Debug.Log("Failed to create new EHS FB object, as its ID is invalid.");
        }
        return null;
    }

    public void DestroyObject(string id)
    {
        if(ehsFbObjects.ContainsKey(id)){
            EHS_FBObject ehsFbObject = ehsFbObjects[id];
            if(ehsFbObject != null){
                functionBlockHandler.DestroyObject(ehsFbObject);
                ehsFbObjects.Remove(id);
            }
        }else{
            Debug.Log("Failed to destroy EHS FB object, as ID: " + id + " doesn't exist.");
        }
    }

    public bool SetMedia(EHS_FunctionBlockConfig config)
    {
        if(ehsFbObjects.ContainsKey(config.id)){
            EHS_FBObject ehsFbObject = ehsFbObjects[config.id];
            if(ehsFbObject != null){
                return functionBlockHandler.SetMedia(ehsFbObject, config);
            }
        }else{
            Debug.Log("Failed as object ID: " + config.id + " doesn't exist.");
        }
        return false;
    }

    public bool StopMedia(EHS_FunctionBlockConfig config)
    {
        if(ehsFbObjects.ContainsKey(config.id)){
            EHS_FBObject ehsFbObject = ehsFbObjects[config.id];
            if(ehsFbObject != null){
                functionBlockHandler.StopMedia(ehsFbObject, config);
            }
        }else{
            Debug.Log("Failed as object ID: " + config.id + " doesn't exist.");
        }
        return false;
    }

    public EHS_FBObject GetObject(string id)
    {
        if(ehsFbObjects.ContainsKey(id)){
            return ehsFbObjects[id];
        }else{
            Debug.Log("Failed to get EHS FB object, ID " + id + " doesn't exist.");
        }
        return null;
    }

    // Scene Manager handling function
    //
    public EHS_SceneManager GetSceneManager()
    {
        return manager;
    }

    public void AddSceneCommand(string command)
    {
        if(!manager.AddCommand(command)){
            Debug.Log("Failed to add command.");
        }
    }

    public void AddSceneCommand(EHS_CommandConfig command)
    {
        if(!manager.AddCommand(command)){
            Debug.Log("Failed to add command.");
        }
    }

    private class DestroyAllObjectsBeforeExternalMediaListener : IObjectDestroyListener {

        private int destroyedCount = 0;
        private int expectedCount = 0;

        public void OnDestroy(EHS_Object ehsObject) {
            lock(this){
                destroyedCount++;
            }
            Debug.Log("Presenation object ("+ehsObject.GetId()+") destroyed. [Total destroyed "+destroyedCount+" out of "+expectedCount+"]");
        }

        public bool Wait(int timeout){ 
            EHS_Utils.Wait(timeout, delegate(){ return AreObjectsDestroyed(); }, 1000/*[ms]*/);
            return AreObjectsDestroyed();
        }

        public void SetExpectedCount(int expectedCount){ 
            this.expectedCount = expectedCount;    
        }

        private bool AreObjectsDestroyed(){
            lock(this){
                return (destroyedCount >= expectedCount);
            }
        }
    }

    private bool PreProcessCommand(EHS_FunctionBlockConfig config){ 
        // do any sort of pre-processing. return 'true' to continue or 'false' to prevent
        // from adding the command to the queue.
        try{
            // handle external media source
            var external = EHS_ExternalMediaSource.GetInstance();
            bool isExternalConfig = external.IsExternal(config);
            bool isExternalActive = external.IsActive();
            // request to stop all widgets, when lauching external media for the first time
            if(isExternalConfig && !isExternalActive){
                var objects = manager.GetParentObjects();
                if(objects.Count > 0){
                    Debug.Log("[Start] Destroy all of the existing unity objects.");
                    var destroyListener = new DestroyAllObjectsBeforeExternalMediaListener();
                    destroyListener.SetExpectedCount(objects.Count);
                    foreach(var ehsObject in objects){ 
                        var listener = ehsObject.GetDestroyListener() as EHS_PresentationHandler.PresentationDestroyListener;
                        if(listener != null){ 
                            listener.SetDestroyListener(destroyListener);
                        }
                        // request object destroy
                        var destroyConfig = EHS_FunctionBlockConfig.Create(ehsObject.GetId(), EHS_FunctionBlockCommand.DESTROY_COMMAND);
                        commands.Push(new EHS_FunctionBlockCommand(this, manager, destroyConfig));
                    }
                    if(!destroyListener.Wait(10000)){ 
                        Debug.LogWarning("Failed to destroy all objects before running the external media");
                    }
                    Debug.Log("[End] Destroy all of the existing unity objects.");
                }
            }
            // process the external media player
            if(isExternalConfig || isExternalActive){ 
                return external.Process(config); 
            }
        }catch(System.Exception e){ 
            Debug.LogException(e);
        }
        return true;   
    }

    private List<ICommand> GetPostProcessCommands(EHS_FunctionBlockConfig config)
    {
        List<ICommand> list = new List<ICommand>();
        // handle post-process of create command
        if(config != null && EHS_FunctionBlockCommand.CREATE_COMMAND.Equals(config.cmd)){
            // handle a special case for creation of the quad widget
            // (1) - Make sure that quads are always shown after they've been created
            if(config.parameters != null && EHS_PresentationType.Quad.Equals(config.parameters.type)){ 
                var setMediaConfig = EHS_FunctionBlockConfig.Create(config.id, EHS_FunctionBlockCommand.SET_MEDIA_COMMAND,
                                                                    EHS_ObjectType.Text, "", "", null);
                list.Add(new EHS_FunctionBlockCommand(this, manager, setMediaConfig));
            }
        }
        return list;    
    }
}
