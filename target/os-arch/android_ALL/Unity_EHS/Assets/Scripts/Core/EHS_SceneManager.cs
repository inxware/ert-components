
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public interface ISceneManagerListener
{
    void OnCreated(EHS_Object ehsObject);
    void OnCreateFailed(string id);
    // return true if destroy completed
    // return false to call OnPostDestroy 
    bool OnDestroyed(string id);
    // gets called if destroy process not completed
    void OnPostDestroy(string id);
}

public class EHS_SceneManager : MonoBehaviour
{
    // list stores all ehs prefabs (Note they are added via unity editor)
    public List<GameObject> EHSPrefabs;
    // list stores all available ehs fonts
    public List<Font> EHSFonts;
    // list stores all available ehs fonts
    public List<TMP_FontAsset> EHSProFonts;
    // list stores all available glowing font materials
    public List<Material> EHSProFontsGlowMaterials;
    // contains map of the text mesh pro fonts
    private Dictionary<string, TMP_FontAsset> ehsProFontsMap;
    // main camera object
    private Camera mainCamera;
    // maps the prefab type name to the index of the prefab list
    private EHS_PrefabMap prefabMap;
    // stores pointers to all succesfully created ehs objects
    private Dictionary<string/*id*/, EHS_Object/*object*/> ehsObjects;
    // contains live ehs commands
    private CommandQueue commands;
    // object update handler 
    private EHS_ObjectUpdate ehsObjectUpdate;
    // event handler used for attaching events to the objects
    private EHS_EventHandler eventHandler;
    // stores list of scene manager listeners for a specific object id
    private Dictionary<string/*id*/, List<ISceneManagerListener>> sceneManagerListeners;
    // this flag is set to true to prevent processing next command.
    private bool locked = false;

    // Start is called before the first frame update
    void Start()
    {
        InitProFont();
        sceneManagerListeners = new Dictionary<string, List<ISceneManagerListener>>();
        mainCamera = Camera.main;
        prefabMap = new EHS_PrefabMap(EHSPrefabs);
        ehsObjects = new Dictionary<string, EHS_Object>();
        commands = new CommandQueue();
        ehsObjectUpdate = new EHS_ObjectUpdate(this);
        eventHandler = new EHS_EventHandler(this);
        locked = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(!IsLocked() && commands.Ready()){
            ICommand command = commands.Grab();
            command.Exec();
        }
    }

    public bool IsLocked(){ 
        return locked;    
    }

    public void Lock(){
        Debug.Log("[LOCK] EHS Scene Manager");
        locked = true;
    }

    public void Unlock(){
        Debug.Log("[UNLOCK] EHS Scene Manager");
        locked = false;
    }

    public void AddListener(string id, ISceneManagerListener listener)
    {
        if(listener != null){
            if(!sceneManagerListeners.ContainsKey(id)){
                var list = new List<ISceneManagerListener>();
                list.Add(listener);  
                sceneManagerListeners.Add(id, list);
            }else{
                var list = sceneManagerListeners[id];
                list.Add(listener);
            }
        }
    }

    public List<ISceneManagerListener> GetListeners(string id){ 
        if(sceneManagerListeners.ContainsKey(id)){
            return sceneManagerListeners[id];
        }
        return null;
    }

    public GameObject GetPrefab(string type)
    {
        return prefabMap.GetPrefab(type);
    }

    public Font GetFont(string fontName){
        try{
            foreach(var font in EHSFonts){ 
                foreach(var name in font.fontNames){ 
                    if(fontName.ToLower().Equals(name.ToLower())){ 
                        return font;
                    }
                }
            }
        }catch(System.Exception e){
            Debug.LogException(e);
        }
        return null;
    }

    private void InitProFont(){ 
        try{ 
            string fontList = "";
            ehsProFontsMap = new Dictionary<string, TMP_FontAsset>();
            foreach(var font in EHSProFonts){
                string fontName = font.faceInfo.familyName;
                ehsProFontsMap.Add(fontName.ToLower(), font);
                fontList += (", "+fontName); // just for display
            }
            Debug.Log("Available fonts: " + fontList);
        }catch(System.Exception e){ 
            Debug.LogException(e);
        }    
    }

    public TMP_FontAsset GetProFont(string fontName){
        try{
            return ehsProFontsMap[fontName];
        }catch(System.Exception e){
            Debug.LogException(e);
        }
        return null;
    }

    public Material GetProFontGlowMaterial(int index){
        try{
            return EHSProFontsGlowMaterials[index];
        }catch(System.Exception e){
            Debug.LogException(e);
        }
        return null;
    }

    public List<Font> GetFontList(){
        return EHSFonts;
    }

    public List<string> GetFontNameList(){
        var list = new List<string>();
        try{
            foreach(var font in EHSFonts){ 
                foreach(var name in font.fontNames){ 
                    list.Add(name);
                    break;
                }
            }
        }catch(System.Exception e){
            Debug.LogException(e);
            list.Clear();
        }
        return list;
    }

    //
    // Command Manager functions
    //
    public bool AddCommand(string jsonCommand){
        Command command = CommandFactory.Create(this, jsonCommand);
        if(command != null){
            commands.Push(command);
            return true;
        }
        return false;
    }

    public bool AddCommand(EHS_CommandConfig commandConfig){
        Command command = CommandFactory.Create(this, commandConfig);
        if(command != null){
            commands.Push(command);
            return true;
        }
        return false;
    }

    //
    // Object Manager functions
    //
    public EHS_Object CreateObject(EHS_ObjectConfig config)
    {
        if(config == null){
            Debug.LogWarning("Attempting to create EHS object witth null config.");
            return null;
        }
        string id = config.id;
        if(!string.IsNullOrEmpty(id) && !ehsObjects.ContainsKey(id)){
            var ehsObject = new EHS_Object(this, config);
            EHS_Object ehsParnetObject = null;
            if(!string.IsNullOrEmpty(config.parentID) && ehsObjects.ContainsKey(config.parentID)){
                ehsParnetObject = ehsObjects[config.parentID];
            }
            GameObject unityObject = ehsObject.Create(ehsParnetObject);
            if(unityObject != null){
                if(ehsObjectUpdate.Update(ehsObject, config)){
                    ehsObjects.Add(id, ehsObject);
                    // Attach new object to a parent   
                    if(ehsParnetObject != null){
                        GameObject unityParnetObject = ehsParnetObject.GetGameObject();
                        // make a position of the child object relative to parent
                        unityObject.transform.parent = unityParnetObject.transform;
                        unityObject.transform.position += unityParnetObject.transform.position;
                        unityObject.transform.localScale = new Vector3(
                            unityParnetObject.transform.localScale.x * unityObject.transform.localScale.x,
                            unityParnetObject.transform.localScale.y * unityObject.transform.localScale.y,
                            unityParnetObject.transform.localScale.z * unityObject.transform.localScale.z);
                    }
                    // Notify about object being created
                    if(sceneManagerListeners.ContainsKey(id)){
                        foreach(var listener in sceneManagerListeners[id]){
                            listener.OnCreated(ehsObject);
                        }
                    }
                    return ehsObject;
                }else{
                    Destroy(unityObject);
                    Debug.Log("Failed to create new object (" + id + ").");
                }
            }
            // Notify about object failed to create
            if(sceneManagerListeners.ContainsKey(id)){
                foreach(var listener in sceneManagerListeners[id]){
                    listener.OnCreateFailed(id);
                }
            }
        }else{
            Debug.Log("Failed to create new object, as ID (" + id + ") already exists or is invalid.");
        }
        return null;
    }

    public void DestroyObject(string id)
    {
        if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
            EHS_Object ehsObject = ehsObjects[id];
            if(ehsObject != null){
                ehsObject.Destroy();
                Destroy(ehsObject.GetGameObject());
                ehsObjects.Remove(id);
                if(sceneManagerListeners.ContainsKey(id)){
                    List<ISceneManagerListener> postDestroy = new List<ISceneManagerListener>();
                    foreach(var listener in sceneManagerListeners[id]){
                        if(!listener.OnDestroyed(id)){
                            postDestroy.Add(listener); 
                        }
                    }
                    sceneManagerListeners.Remove(id);
                    // call post destroy
                    foreach(var listener in postDestroy){
                        listener.OnPostDestroy(id);
                    }
                }   
            }
        }
    }

    public bool UpdateObject(EHS_ObjectConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return ehsObjectUpdate.Update(ehsObject, config);
                }
            }
        }
        return false;
    }

    public bool StopObject(EHS_ObjectConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return ehsObjectUpdate.Stop(ehsObject, config);
                }
            }
        }
        return false;
    }

    public bool SetObjectCallBack(string id, IObjectListener listener)
    {
        if(!string.IsNullOrEmpty(id) && listener != null){
            if(ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    ehsObject.SetCallBack(listener);
                    return true;
                }
            }
        }
        return false;
    }

    public bool GetObjectChildren(string id, List<EHS_Object> list)
    { 
        if(list != null && ehsObjects.ContainsKey(id)){ 
            list.Clear();
            var ehsParentObject = ehsObjects[id];
            if(ehsParentObject != null && ehsParentObject.isParent()){
                foreach(var valuePair in ehsObjects){ 
                    var ehsObject = valuePair.Value;
                    if((ehsObject != null) && (ehsObject.GetParent() == ehsParentObject)){
                        list.Add(ehsObject);
                    }
                }
                return (list.Count > 0);
            }
        }
        return false;
    }

    public EHS_Object GetObject(string id)
    {
        if(ehsObjects.ContainsKey(id)){ 
            return ehsObjects[id];
        }
        return null;    
    }

    public List<EHS_Object> GetParentObjects(){ 
        var list = new List<EHS_Object>();
        try{
            foreach(var pair in ehsObjects){ 
                EHS_Object ehsObject = pair.Value;
                if(ehsObject.isParent()){
                    list.Add(ehsObject);
                }
            }
        }catch(System.Exception e){ 
            Debug.LogException(e);
            list.Clear();
        }
        return list;
    }

    //
    // Event Manager functions
    //
    public bool AttachEvent(EHS_EventConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    eventHandler.AttachEvent(ehsObject, config);
                }
            }
        }
        return false;
    }

    public bool RemoveEvent(EHS_EventConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    eventHandler.RemoveEvent(ehsObject, config);
                }
            }
        }
        return false;
    }

    public bool SetEventCallBack(string id, string ehsEventID, IBehaviourListener listener)
    {
        if(!string.IsNullOrEmpty(id) && listener != null){
            if(ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return eventHandler.SetCallBack(ehsObject, ehsEventID, listener);
                }
            }
        }
        return false;
    }

    //
    // Media Source Manager functions
    //
    public bool CreateMediaSource(EHS_MediaSourceConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return ehsObject.GetMediaSourceHandler().Create(config);
                }
            }
        }
        return false;
    }

    public bool DestroyMediaSource(EHS_MediaSourceConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return ehsObject.GetMediaSourceHandler().Destroy(config);
                }
            }
        }
        return false;
    }

    public bool UpdateMediaSource(EHS_MediaSourceConfig config)
    {
        if(config != null){
            string id = config.id;
            if(!string.IsNullOrEmpty(id) && ehsObjects.ContainsKey(id)){
                EHS_Object ehsObject = ehsObjects[id];
                if(ehsObject != null){
                    return ehsObject.GetMediaSourceHandler().Update(ehsObject, config);
                }
            }
        }
        return false;
    }

    public Camera GetCameraView(){ 
        return mainCamera;
    }
}
