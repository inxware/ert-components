using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_TextureContainer
{
    private Dictionary<string, Texture2D> imageTextures;
    private Dictionary<Texture2D, Texture2D[,]> imageGridTextures;

    public EHS_TextureContainer()
    {
        imageTextures = new Dictionary<string, Texture2D>();
        imageGridTextures = new Dictionary<Texture2D, Texture2D[,]>();
    }

    public Texture2D Get(string id)
    {
        if(Exists(id)){
            return imageTextures[id];
        }
        return null;
    }

    public Texture2D[,] GetGrid(string id)
    {
        if(GridExists(id)){
            return imageGridTextures[Get(id)];
        }
        return null;
    }

    public Texture2D[,] GetGrid(Texture2D texture)
    {
        if(GridExists(texture)){
            return imageGridTextures[texture];
        }
        return null;
    }

    public void Add(string id, Texture2D texture)
    {
        imageTextures.Add(id, texture);
    }

    public void AddGrid(string id, Texture2D[,] gridTexture)
    {
        AddGrid(Get(id), gridTexture);
    }

    public void AddGrid(Texture2D texture, Texture2D[,] gridTexture)
    {
        if(texture != null){
            imageGridTextures.Add(texture, gridTexture);
        }
    }

    public bool Exists(string id)
    {
        return imageTextures.ContainsKey(id);
    }

    public bool GridExists(string id)
    {
        if(Exists(id)){
            return GridExists(Get(id));
        }
        return false;
    }

    public bool GridExists(Texture2D texture)
    {
        if(texture != null){
            return imageGridTextures.ContainsKey(texture);
        }
        return false;
    }

    public void Clear()
    {
        // Destroy grids
        foreach(var gridPair in imageGridTextures){
            DestroyGrid(gridPair.Value);
        }
        imageGridTextures.Clear();
        // Destroy textures
        foreach(var texture in imageTextures){
            Destroy(texture.Value);
        }
        imageTextures.Clear();
    }

    private void DestroyGrid(Texture2D[,] gridTexture)
    {
        foreach(var texture in gridTexture){
            Destroy(texture);
        }
    }

    private void Destroy(Texture2D texture)
    {
        if(texture != null){
            Texture2D.DestroyImmediate(texture, true);
        }
    }
}
