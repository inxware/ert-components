using UnityEditor;

public class BuildScript 
{
    static void Android(){
        string path = System.Environment.GetEnvironmentVariable("UNITY_ANDROID_BATCH_BUILD_PATH");    
        string[] scenes = { "Assets/Scenes/SimpleSignageOnline.unity" };
        BuildPipeline.BuildPlayer(scenes,  path, BuildTarget.Android, BuildOptions.AcceptExternalModificationsToPlayer );
    }
}
