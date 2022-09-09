
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class EHS_PluginHandler : EHS_ICommandsProcessor
{

    [DllImport ("native-activity")]
    private static extern void EhsUnityStartAsync ();

    //we have to do a DLLImport for every function we use
    [DllImport ("native-activity")]
    private static extern bool EhsUnityWritePoll (out IntPtr buffer, out IntPtr length);

    [DllImport ("native-activity")]
    private static extern bool EhsUnityEvent (string id,string msg);

    private static EHS_PluginHandler s_instance;

    public static EHS_PluginHandler GetInstance(){ 
        if(s_instance == null){ 
            s_instance = new EHS_PluginHandler();
        }
        return s_instance;
    }

    private EHS_PluginHandler(){ 
        
    }

    public void Start(){ 
        EhsUnityStartAsync();
    }

    public void Read(EHS_FunctionBlockManager manager){ 
        IntPtr bufferPtr = IntPtr.Zero;
        IntPtr lengthPtr = IntPtr.Zero;
        bool success=EhsUnityWritePoll(out bufferPtr, out lengthPtr);
        while(success){
            int length=lengthPtr.ToInt32();
            if(length>0){
                byte[] data = new byte[length];
                Marshal.Copy(bufferPtr, data, 0, length);
                string command = EHS_Utils.FixJson(System.Text.Encoding.UTF8.GetString(data, 0, data.Length));
                Debug.LogWarning(command);
                manager.AddCommand(command);
            }
            success=EhsUnityWritePoll(out bufferPtr, out lengthPtr);
        }
    }

    public void Write(EHS_FBResponse response){
        if(response != null){
            //EhsUnityEvent(response.GetId(), response.GetMessage());
            Debug.LogWarning("EHS Write ==> Response: id(" + response.GetId() + "), message(" +response.GetMessage()+ ")");
        }
    }
}
