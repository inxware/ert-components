using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_SetActiveHandler : EHS_ObjectHandler
{
    public EHS_SetActiveHandler(EHS_SceneManager manager) : base(manager)
    {
    }

    public override bool Update(EHS_Object ehsObject, EHS_ObjectConfig config)
    {
        if(ehsObject != null  && config != null){
            bool isActive = config.active;
            ehsObject.GetGameObject().SetActive(isActive);
            Debug.Log("Set object ("+ehsObject.GetId()+") : " + ((isActive) ? "Active" : "Inactive"));
            return true;
        }
        return false;
    }
}
