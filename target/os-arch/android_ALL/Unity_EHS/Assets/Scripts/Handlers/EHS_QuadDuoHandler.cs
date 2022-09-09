using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_QuadDuoHandler : EHS_ObjectHandler
{
    public static readonly string AllQuadSides = "front,back";

    public EHS_QuadDuoHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object details handlers
        // (1)
        EHS_QuadObjectSurfaceHandler.Config surfaceConfig = new EHS_QuadObjectSurfaceHandler.Config();
        surfaceConfig.allSides = AllQuadSides;
        objectDetailHandlers.Add(EHS_DetailType.Surface, new EHS_QuadObjectSurfaceHandler(manager, surfaceConfig));
        // (2) ...
        // ...
    }
}
