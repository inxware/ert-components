
using System;
using UnityEngine;

public class EHS_CommandsHandler : MonoBehaviour
{
    private static bool DEBUG_SCREEN_SETTINGS = true;

    private static void OutputScreenSettings(System.IO.StreamWriter writer){
        if(writer == null){ 
            return;
        }
        writer.WriteLine("Screen Settings:");
        writer.WriteLine("currentResolution: " + Screen.currentResolution);
        writer.WriteLine("width: " + Screen.width);
        writer.WriteLine("height: " + Screen.height);
        writer.WriteLine("cutouts: ");
        foreach(var rect in Screen.cutouts){ 
            writer.WriteLine(" " + rect);
        }
        writer.WriteLine("dpi: " + Screen.dpi);
        writer.WriteLine("fullScreen: " + Screen.fullScreen);
        writer.WriteLine("fullScreenMode: " + Screen.fullScreenMode);
        writer.WriteLine("safeArea: " + Screen.safeArea);
        writer.WriteLine("===========================================");
    }

    private static void OutputDisplaySettings(System.IO.StreamWriter writer){
        if(writer == null){ 
            return;
        }
        writer.WriteLine("Display Settings:");
        writer.WriteLine("renderingWidth: " + Display.main.renderingWidth);
        writer.WriteLine("renderingHeight: " + Display.main.renderingHeight);
        writer.WriteLine("systemWidth: " + Display.main.systemWidth);
        writer.WriteLine("systemHeight: " + Display.main.systemHeight);
        writer.WriteLine("===========================================");
    }

    public enum Mode{ 
        Plugin,
        File,
        Network
    }

    private static Mode SelectedMode = Mode.Plugin;

    public EHS_FunctionBlockManager manager;
    public Mode mode = Mode.Plugin;
    private ProcessCommands processCommands;
    private EHS_ExternalMediaSource externalMediaSource;

    // Start is called before the first frame update
    void Start()
    {
        //The simplest way to start controlling frame rate is to explicitly set the QualitySettings. 
        //vSyncCount so that rendering will occur at an interval related to the display device's refresh rate
        //(e.g.for a 60Hz display, setting vSyncCount=2 will cause Unity to render at 30fps in sync with the display).
        QualitySettings.vSyncCount = 2; // comment this out to disable it

        // Set cursor to not be visible
        Cursor.visible = false;

        SelectedMode = mode;
        // prevent android devices from going into sleep mode
        Screen.sleepTimeout = (int)SleepTimeout.NeverSleep;
        // modify resolution
#if UNITY_STANDALONE_WIN && !UNITY_EDITOR_WIN
        System.IO.StreamWriter writer = (DEBUG_SCREEN_SETTINGS) ? new System.IO.StreamWriter("./resolution_change.txt", false) : null;
        try{
            // write settings before changing resolution
            OutputScreenSettings(writer);
            OutputDisplaySettings(writer);
            if(Display.main.renderingWidth != Display.main.systemWidth || Display.main.renderingHeight != Display.main.systemHeight){
                writer.WriteLine("Chanage Rendering Resolution:");
                bool changed = false;
                foreach (var resolution in Screen.resolutions){
                    if(writer != null){
                        writer.WriteLine(resolution.ToString());
                    }
                    if(!changed && (resolution.width == Display.main.systemWidth) && (Display.main.systemHeight == resolution.height)){ 
                        writer.WriteLine("*** Changing Resolution to: " + resolution);
                        Screen.SetResolution(resolution.width, resolution.height, Screen.fullScreenMode, resolution.refreshRate);
                        Display.main.SetRenderingResolution(resolution.width, resolution.height);
                        changed = true;
                    }   
                }
                // write settings after changing resolution
                OutputScreenSettings(writer);
                OutputDisplaySettings(writer);
            }
        }catch(Exception e){
            Debug.Log(e.ToString());
            if(writer != null){
                writer.WriteLine("Resolution change exception:");
                writer.WriteLine(e.ToString());
            }
        }
        if(writer != null){
            writer.Close();
        }
#endif
        Debug.Log("Start EHS");
        externalMediaSource = EHS_ExternalMediaSource.GetInstance();
        externalMediaSource.Start();
        processCommands = new ProcessCommands(manager);
        processCommands.Start();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnDisable()
    { 
        if(processCommands != null){ 
            processCommands.Stop();    
        }
        if(externalMediaSource != null){ 
            externalMediaSource.Stop();    
        }
    }

    private class ProcessCommands : EHS_Runnable { 

        private EHS_FunctionBlockManager manager;
        private EHS_ICommandsProcessor commandsProcessor;

        public ProcessCommands(EHS_FunctionBlockManager manager){ 
            this.manager = manager;
            switch(EHS_CommandsHandler.SelectedMode){ 
                case Mode.File: 
                    commandsProcessor = EHS_UnityTest.GetInstance();
                    break;
                case Mode.Plugin:
                    commandsProcessor = EHS_PluginHandler.GetInstance();
                    break;
                case Mode.Network:
                    // @TODO
                    break;
            };
        }

        protected override void OnStart(){
            Debug.LogWarning("[Start] EHS Commands Processor");
            commandsProcessor.Start();
        }

        protected override void OnStop(){  
            Debug.LogWarning("[Stop] EHS Commands Processor");
        }

        protected override void Run(){ 
            try{
                // read commands from ehs
                commandsProcessor.Read(manager);
                // handle unity responses
                var queue = manager.GetResponseQueue();
                if(queue != null && queue.Ready()){
                    EHS_FBResponse response = queue.Grab();
                    commandsProcessor.Write(response); 
                }
            }catch(Exception e){ 
                Debug.LogException(e);
            }
            EHS_Runnable.Sleep(2);
        }
    }
}
