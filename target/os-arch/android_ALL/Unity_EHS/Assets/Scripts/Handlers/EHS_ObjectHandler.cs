
using System.Collections.Generic;
using UnityEngine;

public interface IObjectDetailValuesHandler
{
    bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs);
}

public class ObjectDetailHandler
{
    protected EHS_SceneManager manager;
    protected Dictionary<string/*name*/, IObjectDetailValuesHandler> objectDetailValuesHandlers;

    public ObjectDetailHandler(EHS_SceneManager manager)
    {
        this.objectDetailValuesHandlers = new Dictionary<string/*name*/, IObjectDetailValuesHandler>();
        this.manager = manager;
    }

    public virtual bool Update(EHS_Object ehsObject, EHS_DetailConfig detail)
    {
        if(detail != null){
            bool success = true;
            if(detail.configs != null && !string.IsNullOrEmpty(detail.id)){
                if(objectDetailValuesHandlers.ContainsKey(detail.id)){
                    IObjectDetailValuesHandler handler = objectDetailValuesHandlers[detail.id];
                    success = ((handler != null) && handler.Update(ehsObject, detail.type, detail.id, detail.configs)) && success;
                }else{
                    Debug.LogWarning("Detail value handler id (" + detail.id + ") not registered!");
                }
            }
            return success;
        }
        return false;
    }

    public virtual bool Stop(EHS_Object ehsObject)
    {
        if(ehsObject != null){
            Debug.Log("Stop NOT impemented");
        }
        return true;
    }
}

public class EHS_ObjectHandler
{
    protected EHS_SceneManager manager;

    protected Dictionary<string/*type*/, ObjectDetailHandler/*handler*/> objectDetailHandlers;

    public EHS_ObjectHandler(EHS_SceneManager manager)
    {
        this.objectDetailHandlers = new Dictionary<string/*type*/, ObjectDetailHandler/*handler*/>();
        this.manager = manager;
    }

    public virtual bool Update(EHS_Object ehsObject, EHS_ObjectConfig config)
    {
        bool success = (config != null);
        if(success){
            // generic update
            EHS_ObjectConfig.UpdateObject(ehsObject, config);
            // detailed update
            EHS_DetailConfig [] details = config.details;
            if(details != null){
                foreach(EHS_DetailConfig detail in details){
                    if(objectDetailHandlers.ContainsKey(detail.type)){
                        ObjectDetailHandler handler = objectDetailHandlers[detail.type];
                        success = (handler != null) && handler.Update(ehsObject, detail);
                        if(!success){
                            Debug.Log("Failed to update a detail (config id: " + config.id + ")");
                            break;
                        }
                    }else{
                        Debug.Log("Detail type (" + detail.type + ") not registered!");
                    }
                }
            }
        }
        return success;
    }

    public virtual bool Stop(EHS_Object ehsObject)
    {
        if(ehsObject != null){
            bool success = true;
            foreach(var handlerPair in objectDetailHandlers){
                var handler = handlerPair.Value;
                if(handler != null && !handler.Stop(ehsObject)){ 
                    success = false;
                    Debug.Log("Failed to stop object(" +ehsObject.GetId()+ ") for type:"+handlerPair.Key);
                }
            }
            return success;
        }
        return false;
    }
}
