
using System;
using UnityEngine;

public class EHS_CommandsHandler : MonoBehaviour
{
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
        SelectedMode = mode;
        // prevent android devices from going into sleep mode
        Screen.sleepTimeout = (int)SleepTimeout.NeverSleep;
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
            EHS_Runnable.Sleep(20);
        }
    }
}
