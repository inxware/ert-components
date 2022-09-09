using System;
using UnityEngine;

public class EHS_TextMeshHandler : ITextObjectHandler
{
    public static readonly float DEFAULT_FONT_SIZE = 200f;
    public static readonly float DEFAULT_SIZE = 0.05f;

    private TextMesh textMesh;
    private EHS_TextObjectConfig objectConfig;

    public EHS_TextMeshHandler(TextMesh textMesh){ 
        if(textMesh == null){ 
            throw new Exception("Unable to create TextMesh object handler due to TextMesh object being null.");    
        }
        this.textMesh = textMesh;
    }

    public ITextObjectHandler Init(EHS_TextObjectConfig objectConfig){ 
        if(objectConfig == null){ 
            throw new Exception("Unable to create TextMesh object handler due to EHS_TextObjectConfig object being null.");    
        }
        this.objectConfig = objectConfig;
        return this;
    }

    public ITextObjectHandler Update(EHS_TextConfig textConfig, string text){ 
        if(textConfig == null){ 
            throw new Exception("Unable to create TextMesh object handler due to EHS_TextConfig object being null.");    
        }
        int fontSize = (int)(DEFAULT_FONT_SIZE * textConfig.fontSize/2f);
        SetFontSize(fontSize);
        SetTextAlignment(textConfig.alignment);
        SetFontStyle(textConfig.fontStyle);
        SetFontType(textConfig.fontType);
        SetText(EHS_Utils.FixText(text));
        var color = textConfig.color;
        if(!string.IsNullOrEmpty(text) && !string.IsNullOrEmpty(color)){
            SetColor(EHS_Utils.PareseColor(color, Color.white));
        }
        return this;
    }

    public ITextObjectHandler Clear(){
        if(textMesh != null){
            textMesh.text = "";
        }
        return this;
    }

    public void SetColor(Color color) {
        var renderer = textMesh.gameObject.GetComponent<Renderer>();
        var materialContainer = objectConfig.ehsObject.GetMaterialContainer();
        string materialKey = objectConfig.ehsObject.GetId() + "_FGRD_Color_" + objectConfig.side;
        Material material = materialContainer.Get(materialKey);
        if(material == null){  // create new material
            material = new Material(renderer.sharedMaterial);
            materialContainer.Add(materialKey, material);
            renderer.sharedMaterial = material;
        }
        material.color = color;
    }

    public void SetFontSize(int fontSize) {
        if(textMesh.fontSize != fontSize){
            textMesh.fontSize = fontSize;
        }      
    }

    public void SetFontStyle(string textStyle) {
        if(!string.IsNullOrEmpty(textStyle)){
            FontStyle fontStyle = FontStyle.Normal;
            if(textStyle.Equals("normal")){
                fontStyle = FontStyle.Normal;
            }else if(textStyle.Equals("bold")){
                fontStyle = FontStyle.Bold;
            }else if(textStyle.Equals("italic")){
                fontStyle = FontStyle.Italic;
            }else if(textStyle.Contains("bold") && textStyle.Contains("italic")){
                fontStyle = FontStyle.BoldAndItalic;
            }
            if(textMesh.fontStyle != fontStyle){ 
                textMesh.fontStyle = fontStyle;
            }
        }
    }

    public void SetFontType(string typeName) {
        // check if already set
        try{
            foreach(var name in textMesh.font.fontNames){
                if(name.ToLower().Equals(typeName.ToLower())){ 
                    // font already set
                    return;
                }
            }
            var sceneManager = objectConfig.ehsObject.GetManager();
            var font = sceneManager.GetFont(typeName);
            if(font != null){
                var renderer = textMesh.gameObject.GetComponent<Renderer>();
                var material = font.material;
                textMesh.font = font;
                renderer.material = material;
            }
        }catch(Exception e){ 
            Debug.LogException(e);
        }
    }

    public void SetText(string text) {
         textMesh.text = text;
    }

    public void SetTextAlignment(string textAlignment) {
         if(!string.IsNullOrEmpty(textAlignment)){
            TextAlignment alignment = TextAlignment.Center;
            if(textAlignment.Equals("right")){
                alignment = TextAlignment.Right;
            }else if(textAlignment.Equals("left")){
                alignment = TextAlignment.Left;
            }else if(textAlignment.Equals("center")){
                alignment = TextAlignment.Center;
            }
            if(textMesh.alignment != alignment){
                textMesh.alignment = alignment;
            }
        }
    }

    public static TextMesh GetTextMesh(GameObject unityObject, GameObject textObject, float sizeScale){
        Vector3 scale = Vector3.one*DEFAULT_SIZE/sizeScale;
        // prevent streaching of the text
        Vector3 unityObjectScale;
        unityObjectScale = unityObject.transform.parent.localScale;
        if(unityObjectScale.x > unityObjectScale.y){ 
            scale.x *= (unityObjectScale.y/unityObjectScale.x);         
        }else{ 
            scale.y *= (unityObjectScale.x/unityObjectScale.y);    
        }
        textObject.transform.localScale = scale;
        textObject.transform.localPosition -= Vector3.forward*0.01f;
        return textObject.GetComponent<TextMesh>();
    }
}
