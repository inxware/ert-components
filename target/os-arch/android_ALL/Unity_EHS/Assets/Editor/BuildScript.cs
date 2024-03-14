using UnityEditor;

public class BuildScript 
{
     public static void Android() {
        EditorUserBuildSettings.exportAsGoogleAndroidProject = true;
        BuildPlayerOptions buildPlayerOptions = new BuildPlayerOptions();
        buildPlayerOptions.scenes = new[] { "Assets/Scenes/SimpleSignageOnline.unity" };
        buildPlayerOptions.locationPathName = System.Environment.GetEnvironmentVariable("UNITY_ANDROID_BATCH_BUILD_PATH");
        buildPlayerOptions.target = BuildTarget.Android;
        buildPlayerOptions.options = BuildOptions.None;
        BuildPipeline.BuildPlayer(buildPlayerOptions);
    }
}
