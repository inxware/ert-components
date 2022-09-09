using System.Collections;
using System;
using UnityEngine;
using UnityEngine.Networking;

public class EHS_AudioPlayer : MonoBehaviour
{
    private AudioSource audioSource;

    // Start is called before the first frame update
    void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
    }

    public void Play(string path){
        StartCoroutine(LoadClip(path));
    }

    public void Stop(){ 
        if(audioSource != null){ 
            audioSource.Stop();    
        }
    }

    IEnumerator LoadClip(string path)
    {
        string url = "file://"+path;
        Debug.Log("AUDIO PATH: " + url);
        using (UnityWebRequest uwr = UnityWebRequestMultimedia.GetAudioClip(url, AudioType.UNKNOWN))
        {
            yield return uwr.SendWebRequest();

            if (uwr.isNetworkError || uwr.isHttpError){
                Debug.Log(uwr.error);
            }else{
                 try{
                    AudioClip clip = DownloadHandlerAudioClip.GetContent(uwr);
                    if(clip != null){
                        audioSource.clip = clip; 
                        audioSource.Play();
                    }
                }catch(Exception e){ 
                    Debug.LogException(e);
                }
            }
        }
    }
}
