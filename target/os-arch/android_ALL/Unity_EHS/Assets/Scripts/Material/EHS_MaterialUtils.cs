using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_MaterialUtils
{
    public static void ApplyImage(EHS_Object ehsObject, Texture texture, string side)
    {
        GameObject parent = ehsObject.GetGameObject();
        GameObject quad = EHS_ObjectUtils.GetChildWithName(parent, side);
        EHS_QuadHandler.ApplyImage(ehsObject, quad, texture, side, true);
    }

    public static void ApplyImage(EHS_Object ehsObject, GameObject quad, Texture texture, string sufixId, bool parentScale=false)
    {
        var material = GetMaterial(ehsObject, quad, sufixId);
        ApplyImage(quad, material, texture, parentScale);
    }

    public static void ApplyImage(EHS_Object ehsObject, Texture texture, string sufixId, bool parentScale=false)
    {
        GameObject quad = ehsObject.GetGameObject();
        ApplyImage(ehsObject, quad, texture, sufixId, parentScale);
    }

    public static void ApplyImage(EHS_Object ehsObject, Texture texture, bool parentScale=false)
    {
        ApplyImage(ehsObject, texture, ehsObject.GetId(), parentScale);
    }

    public static void ApplyImage(GameObject quad, Material material, Texture texture, bool parentScale=false)
    {
        material.mainTexture = texture;
        // prevent streaching of the text
        float quadScale;
        if(parentScale){
            var transform = quad.transform.parent;
            float sy = transform.localScale.y;
            quadScale = (sy != 0f) ? transform.localScale.x / sy : 1f;
        }else{
            float sy = quad.transform.localScale.y;
            quadScale = (sy != 0f) ? quad.transform.localScale.x / sy : 1f;
        }
        float textureScale = (float)texture.width/(float)texture.height;
        if(quadScale != 1f || textureScale != 1f){
            float heigth = 1f, width = 1f;

            if(quadScale > textureScale){
                width = quadScale/textureScale;
            }else{ 
                heigth = textureScale/quadScale;
            }
            float offsetX = (1f - width)/2f;
            float offsetY = (1f - heigth)/2f;
            material.SetTextureScale("_MainTex", new Vector2(width, heigth));
            material.SetTextureOffset("_MainTex", new Vector2(offsetX, offsetY));
        }else{ 
            material.SetTextureScale("_MainTex", new Vector2(1f, 1f));
            material.SetTextureOffset("_MainTex", new Vector2(0f, 0f)); 
        }
    }

    public static void ClearTexture(EHS_Object ehsObject, GameObject unityObject, string sufixId){
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetTextureScale("_MainTex", new Vector2(0, 0));
        material.SetTextureOffset("_MainTex", new Vector2(0, 0));
    }

    public static Material GetMaterial(EHS_Object ehsObject, GameObject unityObject, string sufixId){ 
        var renderer = unityObject.GetComponent<Renderer>();
        var materialContainer = ehsObject.GetMaterialContainer();
        string materialKey = ehsObject.GetId() + "_BGRD_Color_" + sufixId;
        Material material = materialContainer.Get(materialKey);
        if(material == null){  // create new material
            material = new Material(renderer.sharedMaterial);
            materialContainer.Add(materialKey, material);
            renderer.sharedMaterial = material;
        }
        return material;
    }

    public static void UpdateBackgroundColor(EHS_Object ehsObject, GameObject unityObject, Color color, string sufixId){
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetColor("_BackgroundColor", color);
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color, string sufixId){ 
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        material.SetColor("_Color", color);
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color){
        UpdateForegroundColor(ehsObject, unityObject, color, "");
    }

    public static void UpdateBackgroundColor(EHS_Object ehsObject, GameObject unityObject, Color color){ 
        UpdateBackgroundColor(ehsObject, unityObject, color, "");
    }

    public static void UpdateColor(EHS_Object ehsObject, GameObject unityObject, string bgrColor, string fgrColor, string sufixId){ 
        var material = GetMaterial(ehsObject, unityObject, sufixId);
        if(!string.IsNullOrEmpty(bgrColor)){
            Color color = EHS_Utils.PareseColor(bgrColor, Color.white);
            material.SetColor("_BackgroundColor", color);
        }
        if(!string.IsNullOrEmpty(fgrColor)){
            Color color = EHS_Utils.PareseColor(fgrColor, Color.white);
            material.SetColor("_Color", color);
        }
    }
}
