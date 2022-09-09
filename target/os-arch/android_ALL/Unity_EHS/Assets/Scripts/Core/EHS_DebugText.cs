using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EHS_DebugText : MonoBehaviour
{
    private Text textObject;

    void OnEnable () {
        Application.logMessageReceived += HandleLog;
    }

    void OnDisable () {
        Application.logMessageReceived -= HandleLog;
    }

    void HandleLog(string logString, string stackTrace, LogType type)
    {
        if(type == LogType.Log || type == LogType.Exception){
            if(textObject != null){
                textObject.text = logString;
            }
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        textObject = gameObject.GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
    }
}
