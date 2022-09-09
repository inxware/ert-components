using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_ObjectUtils
{
    public static readonly string BackSide = "back";
    public static readonly string FrontSide = "front";
    public static readonly string AllSidesString = "top,bottom,left,right,front,back";
    public static readonly string[] AllSidesArray =  AllSidesString.Split(',');

    public static Dictionary<string, string> QuadNameMap = new Dictionary<string, string>
    {
        {"top"   ,"QuadTop"},
        {"bottom","QuadBottom"},
        {"left"  ,"QuadLeft"},
        {"right" ,"QuadRight"},
        {"front" ,"QuadFront"},
        {"back"  ,"QuadBack"}
    };

    public static Dictionary<string, T> CreateSideDictionary<T>(T def){ 
        var dictionary = new Dictionary<string, T>();
        foreach(var side in AllSidesArray){ 
            dictionary.Add(side, def);    
        }
        return dictionary;
    } 

    public static GameObject GetChildWithName(GameObject unityObject, string name) {
        Transform transform = unityObject.transform;
        Transform childTransform = transform.Find(name);
        if (childTransform != null) {
            return childTransform.gameObject;
        } else {
            return null;
        }
    }

    public static void DestroyMaterial(GameObject unityObject) 
    {
        if(unityObject != null){
            foreach(Transform transform in unityObject.transform){
                GameObject child = transform.gameObject;
                DestroyMaterial(child);
                var mesh = child.GetComponent<MeshRenderer>();
                if(mesh != null){
                    var material = mesh.materials[0];
                    if(material != null){
                        GameObject.Destroy(material);
                    }
                }
            }
        }
    }

    public static EHS_CommandConfig CreateObjectCommand(string parent, string id, string type, Vector3 position, Vector3 scale, Quaternion rotation)
    {
        var command = new EHS_CommandConfig("createObject");
        command.objectConfig = new EHS_ObjectConfig(type);
        if(!string.IsNullOrEmpty(parent)){
            command.objectConfig.parentID = parent;
        }
        command.objectConfig.id = id;
        command.objectConfig.position = position;
        command.objectConfig.scale = scale;
        command.objectConfig.rotation = rotation;

        return command;
    }

    public static EHS_CommandConfig CreateObjectCommand(string parent, string id, string type, Vector3 position, Vector3 scale)
    {
        return CreateObjectCommand(parent, id, type, position, scale, Quaternion.identity);
    }

    public static EHS_CommandConfig CreateObjectCommand(string parent, string id, string type, Vector3 position)
    {
        return CreateObjectCommand(parent, id, type, position, Vector3.one);
    }

    public static EHS_CommandConfig CreateObjectCommand(string parent, string id, string type)
    {
        return CreateObjectCommand(parent, id, type, Vector3.zero);
    }

    public static string DestroyObjectCommand(string id)
    {
        return "{\"type\":\"removeObject\",\"id\":\"" + id + "\"}";
    }

    public static string[] GetSides(string sidesString)
    {
        try{
            if(!string.IsNullOrEmpty(sidesString)){
                string[] sides;
                if(!string.IsNullOrEmpty(sidesString) && sidesString.Contains(",")){
                    sides = sidesString.Split(char.Parse(","));
                }else{
                    sides = new string[1];
                    sides[0] = sidesString;
                }
                return sides;
            }
        }catch(Exception e){
            Debug.LogException(e);
        }
        return default(string[]);
    }
}
