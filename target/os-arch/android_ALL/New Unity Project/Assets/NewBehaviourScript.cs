using System;
﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class NewBehaviourScript : MonoBehaviour
{
#if UNITY_IPHONE
   
// On __iOS__ plugins are statically linked into
// the executable, so we have to use __Internal as the
// library name.
[DllImport ("__Internal")]

#else

// Other platforms load plugins dynamically, so pass the name
// of the plugin's dynamic library.
[DllImport ("native-activity")]

#endif
private static extern void EhsUnityStartAsync ();

//we have to do a DLLImport for every function we use
[DllImport ("native-activity")]
private static extern bool EhsUnityWritePoll (out IntPtr buffer, out IntPtr length);

    // Start is called before the first frame update
    void Start()
    {
	EhsUnityStartAsync();
    }

    // Update is called once per frame
    void Update()
    {
	IntPtr bufferPtr = IntPtr.Zero;
	IntPtr lengthPtr = IntPtr.Zero;
	bool success=EhsUnityWritePoll(out bufferPtr, out lengthPtr);
	while(success){
		int length=lengthPtr.ToInt32();
		if(length>0){
			byte[] data = new byte[length];
		      	Marshal.Copy(bufferPtr, data, 0, length);
			string converted = System.Text.Encoding.ASCII.GetString(data, 0, data.Length);
			print("PBB message="+converted);
		}
		success=EhsUnityWritePoll(out bufferPtr, out lengthPtr);
	}
    }
}
