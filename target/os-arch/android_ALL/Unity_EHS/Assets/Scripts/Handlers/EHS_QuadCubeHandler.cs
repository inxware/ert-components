
public class EHS_QuadCubeHandler : EHS_ObjectHandler
{
    public static readonly string AllCubeSides = "top,bottom,left,right,front,back";

    public EHS_QuadCubeHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object details handlers
        // (1)
        EHS_QuadObjectSurfaceHandler.Config surfaceConfig = new EHS_QuadObjectSurfaceHandler.Config();
        surfaceConfig.allSides = AllCubeSides;
        objectDetailHandlers.Add(EHS_DetailType.Surface, new EHS_QuadObjectSurfaceHandler(manager, surfaceConfig));
        // (2) ...
        // ...
    }
}
