using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_ObjectUpdate
{
    private Dictionary<string/*type*/, EHS_ObjectHandler/*handler*/> ehsObjectHandlers;

    public EHS_ObjectUpdate(EHS_SceneManager manager)
    {
        // register object update handlers
        ehsObjectHandlers = new Dictionary<string, EHS_ObjectHandler>();
        // (1) - Cube handler
        ehsObjectHandlers.Add(EHS_ObjectType.Cube, new EHS_CommonDetailHandler(manager));
        // (2) - Matrix handler
        ehsObjectHandlers.Add(EHS_ObjectType.Matrix, new EHS_MatrixHandler(manager));
        // (3) - Sphere handler
        ehsObjectHandlers.Add(EHS_ObjectType.Sphere, new EHS_CommonDetailHandler(manager));
        // (4) - Sparks handler
        ehsObjectHandlers.Add(EHS_ObjectType.Sparks, new EHS_ParticlesHandler(manager));
        // (5) - Text handler
        ehsObjectHandlers.Add(EHS_ObjectType.Text, new EHS_TextHandler(manager));
        // (6) - QuadCube handler
        ehsObjectHandlers.Add(EHS_ObjectType.QuadCube, new EHS_QuadCubeHandler(manager));
        // (7) - Demo handler
        ehsObjectHandlers.Add(EHS_ObjectType.Demo, new EHS_DemoHandler(manager));
        // (8) - Presentation handler
        ehsObjectHandlers.Add(EHS_ObjectType.Presentation, new EHS_PresentationHandler(manager));
        // (9) - Quad handler
        ehsObjectHandlers.Add(EHS_ObjectType.Quad, new EHS_QuadHandler(manager));
        // (10) - Set Active handler
        ehsObjectHandlers.Add(EHS_ObjectType.SetActive, new EHS_SetActiveHandler(manager));
        // (11) - Camera View handler
        ehsObjectHandlers.Add(EHS_ObjectType.View, new EHS_ViewHandler(manager));
        // (12) - Quad Duo handler
        ehsObjectHandlers.Add(EHS_ObjectType.QuadDuo, new EHS_QuadDuoHandler(manager));
        // ...
    }

    public bool Update(EHS_Object ehsObject, EHS_ObjectConfig config)
    {
        if(config != null && ehsObjectHandlers.ContainsKey(config.type)){
            string type = config.type;
            EHS_ObjectHandler handler = ehsObjectHandlers[type];
            if(handler != null){
                if(handler.Update(ehsObject, config)){
                    ehsObject.UpdateState(type, true);
                    return true;
                }
            }
            ehsObject.UpdateState(type, false);
            return false;
        }
        return false;
    }

    public bool Stop(EHS_Object ehsObject, EHS_ObjectConfig config)
    {
        if(ehsObject != null && config != null && 
            ehsObjectHandlers.ContainsKey(config.type)){
            string type = config.type;
            EHS_ObjectHandler handler = ehsObjectHandlers[type];
            if(handler != null && handler.Stop(ehsObject)){
                ehsObject.Stop(type, true);
                return true;
            }
            ehsObject.Stop(type, false);
            return false;
        }
        return false;
    }
}
