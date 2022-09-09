using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class EHS_ObjectCollectionHandler : EHS_ObjectHandler
{
    private EHS_ObjectUpdate objectUpdate;

    public EHS_ObjectCollectionHandler(EHS_SceneManager manager, EHS_ObjectUpdate objectUpdate) : base(manager)
    {
        this.objectUpdate = objectUpdate;
    }

    public bool UpdateCollection(EHS_Object ehsCollectionObject, EHS_ObjectCollectionConfig config)
    {
        // generic update
        bool success = Update(ehsCollectionObject, config);
         // update collection parent object
        if(success){
            // children update
            EHS_ObjectConfig [] collection = config.collection;
            if(collection != null && objectUpdate != null){
                // update all collection children
                foreach(EHS_ObjectConfig objectConfig in collection){
                    // @TODO - get object child from collection
                    EHS_Object ehsObject = null;
                    if(!objectUpdate.Update(ehsObject, objectConfig)){
                        success = false;
                    }
                }
            }
        }
        return success;
    }
}
