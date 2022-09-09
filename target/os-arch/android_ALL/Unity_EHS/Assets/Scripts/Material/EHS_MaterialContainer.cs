using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_MaterialContainer 
{
    private Dictionary<string, Material> materials;

    public EHS_MaterialContainer()
    {
        materials = new Dictionary<string, Material>();
    }

    public Material Get(string id)
    {
        if(Exists(id)){
            return materials[id];
        }
        return null;
    }

    public void Add(string id, Material material)
    {
        materials.Add(id, material);
    }

    public bool Exists(string id)
    {
        return materials.ContainsKey(id);
    }

    public void Clear()
    {
        // Destroy materials
        foreach(var material in materials){
            Destroy(material.Value);
        }
        materials.Clear();
    }

    private void Destroy(Material material)
    {
        if(material != null){
            Material.DestroyImmediate(material, true);
        }
    }
}
