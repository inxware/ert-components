using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_FunctionBlockHandler
{
    private EHS_FunctionBlockManager manager;

    private Dictionary<string, EHS_FunctionBlockObjectHandler> objectHandler;

    public EHS_FunctionBlockHandler(EHS_FunctionBlockManager manager)
    {
        this.manager = manager;
        this.objectHandler = new Dictionary<string, EHS_FunctionBlockObjectHandler>();
        // Register all ehs function block handlers
        this.objectHandler.Add(EHS_FunctionBlockTypes.Presentation, new EHS_FBPresentationHandler(manager));
        // ...
    }

    public bool CreateObject(EHS_FBObject ehsFbObject)
    {
        if(objectHandler.ContainsKey(ehsFbObject.GetFBType())){
            var handler = objectHandler[ehsFbObject.GetFBType()];
            return handler.Create(ehsFbObject);
        }
        return true;
    }

    public void DestroyObject(EHS_FBObject ehsFbObject)
    {
        if(objectHandler.ContainsKey(ehsFbObject.GetFBType())){
            var handler = objectHandler[ehsFbObject.GetFBType()];
            handler.Destroy(ehsFbObject);
        }
    }

    public bool SetMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config)
    {
        if(objectHandler.ContainsKey(ehsFbObject.GetFBType())){
            var handler = objectHandler[ehsFbObject.GetFBType()];
            return handler.SetMedia(ehsFbObject, config);
        }
        return false;
    }

    public bool StopMedia(EHS_FBObject ehsFbObject, EHS_FunctionBlockConfig config)
    {
        if(objectHandler.ContainsKey(ehsFbObject.GetFBType())){
            var handler = objectHandler[ehsFbObject.GetFBType()];
            handler.StopMedia(ehsFbObject, config);
        }
        return false;
    }

    public static EHS_CommandConfig CreateCommand(string id, string type, EHS_FunctionBlockParametersConfig parameters)
    {
        // build create object command
        EHS_CommandConfig command = new EHS_CommandConfig("createObject");

        command.objectConfig = new EHS_ObjectConfig(type);
        command.objectConfig.id = id;
        command.objectConfig.position = new Vector3(parameters.x, parameters.y, parameters.z);
        command.objectConfig.scale = new Vector3(parameters.w, parameters.h, parameters.d);

        return command;
    }
}
