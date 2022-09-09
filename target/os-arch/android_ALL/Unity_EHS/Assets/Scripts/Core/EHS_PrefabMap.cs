using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_PrefabMap
{
    private List<GameObject> ehsPrefabs;
    // maps ehs object type to the ehs manager list index
    private Dictionary<string, int> objectMap;
    // list of empty prefabs
    private List<string> emptyPrefabs;

    public EHS_PrefabMap(List<GameObject> prefabs)
    {
        ehsPrefabs = prefabs;
        objectMap = new Dictionary<string, int>();
        objectMap.Add(EHS_ObjectType.Cube,          0);
        objectMap.Add(EHS_ObjectType.Sphere,        1);
        objectMap.Add(EHS_ObjectType.Matrix,        2);
        objectMap.Add(EHS_ObjectType.Sparks,        3);
        objectMap.Add(EHS_ObjectType.Demo,          4);
        objectMap.Add(EHS_ObjectType.Text,          5);
        objectMap.Add(EHS_ObjectType.QuadCube,      6);
        objectMap.Add(EHS_ObjectType.Presentation,  7);
        objectMap.Add(EHS_ObjectType.VideoPlayer,   8); // video player attached to objects
        objectMap.Add(EHS_ObjectType.Quad,          9);
        objectMap.Add(EHS_ObjectType.Title,        10);
        objectMap.Add(EHS_ObjectType.Empty,        11);
        objectMap.Add(EHS_ObjectType.Audio,        12);
        objectMap.Add(EHS_ObjectType.QuadDuo,      13);
        // ...
        // objectMap.Add(EHS_NextPrefab, N);

        // append types which should be mapped to an empty prefab
        emptyPrefabs = new List<string>();
        emptyPrefabs.Add(EHS_ObjectType.View);
    }

    // return unity game prefab for EHS type
    public GameObject GetPrefab(string type)
    {
        if(objectMap.ContainsKey(type))
        {
            int index = objectMap[type];
            if(ehsPrefabs.Count > index){
                return ehsPrefabs[index];
            }else{
                Debug.Log("No pre-defined prefab has been found!");
            }
        }else if(emptyPrefabs.Contains(type)){ 
            return ehsPrefabs[objectMap[EHS_ObjectType.Empty]];
        }else{
            Debug.LogWarning("Unknown EHS Prefab type : " + type);
        }
        return null;
    }
}
