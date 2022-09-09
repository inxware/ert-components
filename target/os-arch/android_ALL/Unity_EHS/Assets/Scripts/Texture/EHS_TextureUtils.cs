using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_TextureUtils
{
    public static bool UpdateObjectTexture(GameObject ehsObject, Texture texture)
    {
        if(texture != null){
            var mesh = ehsObject.GetComponent<MeshRenderer>();
            var material = mesh.materials[0];
            material.mainTexture = texture;
            return true;
        }else{
            Debug.Log("Failed to update material. Texture is null.");
        }
        return false;
    }

    public static bool UpdateObjectTexture(EHS_Object ehsObject, string path)
    {
        Texture texture = ehsObject.GetTextureContainer().Get(path);
        return UpdateObjectTexture(ehsObject.GetGameObject(), texture);
    }

    public static Texture2D [,] CreateTextureGrid(Texture2D texture, int size)
    {
        Texture2D [,] textureNxN = new Texture2D [size,size];
        int blockWidth = texture.width / size;
        int blockHeight = texture.height / size;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                int x = j * blockWidth;
                int y = i * blockHeight;
                Color[] pix = texture.GetPixels(x, y, blockWidth, blockHeight);
                Texture2D dst = new Texture2D(blockWidth, blockHeight);
                dst.SetPixels(pix);
                dst.Apply();
                int n = (size - 1) - i;
                int m = (size - 1) - j;
                textureNxN[n,m] = dst;
            }
        }
        return textureNxN;
    }

    public static Texture2D [,] UpdateTextureGrid(Texture2D texture, Texture2D [,] textureNxN)
    {
        if(textureNxN != null){
            int size = textureNxN.GetLength(0);
            int blockWidth = texture.width / size;
            int blockHeight = texture.height / size;
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    int x = j * blockWidth;
                    int y = i * blockHeight;
                    Color[] pix = texture.GetPixels(x, y, blockWidth, blockHeight);
                    int n = (size - 1) - i;
                    int m = (size - 1) - j;
                    Texture2D dst = textureNxN[n,m];
                    if(dst != null){
                        dst.SetPixels(pix);
                        dst.Apply();
                        textureNxN[n,m] = dst;
                    }else{
                        Debug.Log("Failed to update texture n("+n+")m("+m+") as it's null.");
                    }
                }
            }
        }else{
            Debug.Log("Failed to update texture as the source is null.");
        }
        return textureNxN;
    }

    public static Texture2D RenderTexture2Texture2D(RenderTexture renderTexture)
    {
        Texture2D texture2D = new Texture2D(renderTexture.width, renderTexture.height);
        if(!RenderTexture2Texture2D(renderTexture, texture2D)){
            Texture2D.Destroy(texture2D);
            return null;
        }
        return texture2D;
    }

    public static bool RenderTexture2Texture2D(RenderTexture renderTexture, Texture2D texture2D)
    {
        if(renderTexture != null){
            bool isActiveChanged = false;
            if(RenderTexture.active == null){
                RenderTexture.active = renderTexture;
                isActiveChanged = true;
            }
            texture2D.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
            texture2D.Apply();
            if(isActiveChanged){
                RenderTexture.active = null;
            }
            return true;
        }
        return false;
    }
}
