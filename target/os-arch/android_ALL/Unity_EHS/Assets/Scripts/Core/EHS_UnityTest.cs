
using UnityEngine;
using System.IO;
using System.Collections.Generic;

public class EHS_UnityTest : EHS_ICommandsProcessor
{
    private static EHS_UnityTest s_instance;
    private ResourceFileReader jsonResourceFiles;
    private int counter;

    private EHS_UnityTest(){
#if UNITY_ANDROID && !UNITY_EDITOR_WIN
        jsonResourceFiles = new ResourceFileReader("/sdcard/Download/json_paths");
#else
        jsonResourceFiles = new ResourceFileReader("Assets/Resources/json_paths");
#endif
    }

    public static EHS_UnityTest GetInstance(){ 
        if(s_instance == null){ 
            s_instance = new EHS_UnityTest();
        }
        return s_instance;
    }

    public void Start(){
        counter = 0;
        jsonResourceFiles.Read();
    }

    public void Read(EHS_FunctionBlockManager manager){
        Tester tester = Tester.GetTester(manager);

        string testerPath = jsonResourceFiles.FileData(counter);
        tester.SetPath(testerPath);

        bool finished=tester.Run();

        if(finished){ 
            tester.Restart();
            counter++;
            if(jsonResourceFiles.FileDataCount() <= counter){ 
                counter = 0;
            } 
        }
    }

    public void Write(EHS_FBResponse response){
        if(response != null){
            Debug.LogWarning("Response: id(" + response.GetId() + "), message(" +response.GetMessage()+ ")");
        }
    }

    class Tester{
        private static readonly float TIMEOUT = 1f; 
        private EHS_FunctionBlockManager manager;
        private EHS_Timeout timeout;
        private static Tester s_instance;
        private string jsonpath;
        private string [] jsonCommands;
        private int counter = 0;

        public static Tester GetTester(EHS_FunctionBlockManager manager){ 
            if(s_instance == null){ 
                s_instance = new Tester(manager);    
            }
            return s_instance;
        }

        public Tester(EHS_FunctionBlockManager manager){
            this.manager = manager;
            this.timeout = new EHS_Timeout();
        }

        public void SetPath(string path){ 
            jsonpath = path;
        }

        public bool Run(){
            if(!timeout.IsStrated()){
                Start();
            }
            if(timeout.Timeout()){
                if(counter >= jsonCommands.Length){
                    return true; // done
                }
                string command = jsonCommands[counter];
                // ignore empty of commented-out lines 
                while(string.IsNullOrEmpty(command) || IsComment(command)){
                    counter++;
                    if(counter >= jsonCommands.Length){
                        return true; // done
                    }
                    command = jsonCommands[counter];
                }
                if(command[0] == 't'){ // update timeout
                    timeout.SetTimeout(float.Parse(command.Split(':')[1]));
                    counter++;
                    if(counter >= jsonCommands.Length){
                        return true; // done
                    }
                    return false;
                }
                Debug.LogWarning(command);
                manager.AddCommand(command);
                counter++;
                timeout.Restart();
            }
            return false;
        }

        public void Restart(){
            timeout.Stop();
            counter = 0;
        }

        private void Start(){
            try{
                jsonCommands = File.ReadAllText(jsonpath).Split('\n');
                timeout.SetTimeout(TIMEOUT);
                timeout.Start();
            } catch(System.Exception e){ 
                Debug.LogException(e);
            }
        }
    }

    private static bool IsComment(string line){ 
        return !string.IsNullOrEmpty(line) && (line[0] == '#');
    }

    private class ResourceFileReader{

        private List<string> fileData;
        private string fileName;
        
        public ResourceFileReader(string fileName){
            this.fileName = fileName;
            this.fileData = new List<string>();
        }

        public bool Read(){ 
            try{
                var lines = File.ReadAllText(fileName).Replace("\r","").Split('\n');
                foreach(string data in lines){
                    if(!string.IsNullOrEmpty(data) && data[0] != '#'){ 
                        fileData.Add(data);
                    }    
                }
                return true;
            } catch(System.Exception e){ 
                Debug.LogException(e);
            }
            return false;
        }
        
        public string[] AllFileData(){ 
            return fileData.ToArray();    
        }

        public int FileDataCount(){ 
            return fileData.Count;
        }

        public string FileData(int index){ 
            if(FileDataCount() > index){ 
                return fileData[index];    
            }
            return null;
        }
    }
}
