using System;
using UnityEngine;

public class EHS_TextHandler : EHS_ObjectHandler
{
    public static readonly float DEFAULT_FONT_SIZE = 200f;
    public static readonly float DEFAULT_SIZE = 0.05f;

    public EHS_TextHandler(EHS_SceneManager manager) : base(manager)
    {
        // register object update handlers
        objectDetailHandlers.Add(EHS_DetailType.Text, new TextDetailHandler(manager));
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color, string sufixId){ 
        var renderer = unityObject.GetComponent<Renderer>();
        var materialContainer = ehsObject.GetMaterialContainer();
        string materialKey = ehsObject.GetId() + "_FGRD_Color_" + sufixId;
        Material material = materialContainer.Get(materialKey);
        if(material == null){  // create new material
            material = new Material(renderer.sharedMaterial);
            materialContainer.Add(materialKey, material);
            renderer.sharedMaterial = material;
        }
        material.color = color;
    }

    public static bool ClearSurfaceText(GameObject quad){
        if(quad == null){
            return false;
        }
        foreach(Transform transform in quad.transform){ 
            if(transform != null){
                GameObject child = transform.gameObject;
                // Destroy text object
                var mesh = child.GetComponent<TextMesh>();
                if(mesh != null){ 
                    //GameObject.DestroyImmediate(child);
                    mesh.text = "";
                }
            }
        }
        return true;
    }

    public static TextMesh GetTextMesh(GameObject quad, GameObject textObject, float sizeScale, bool parentScale=false){
        Vector3 scale = Vector3.one*EHS_TextHandler.DEFAULT_SIZE/sizeScale;
        // prevent streaching of the text
        Vector3 quadScale;
        if(parentScale){ 
            quadScale = quad.transform.parent.localScale;
        }else{ 
            quadScale = quad.transform.localScale;
        }
        if(quadScale.x > quadScale.y){ 
            scale.x *= (quadScale.y/quadScale.x);         
        }else{ 
            scale.y *= (quadScale.x/quadScale.y);    
        }
        textObject.transform.localScale = scale;
        textObject.transform.localPosition -= Vector3.forward*0.01f;
        return textObject.GetComponent<TextMesh>();
    }

    public static void UpdateFontSize(TextMesh mesh, float size, float scale){
        int fontSize = (int)(DEFAULT_FONT_SIZE * size/scale);
        if(mesh.fontSize != fontSize){
            mesh.fontSize = fontSize;
        }      
    }

    public static void UpdateTextAlignment(TextMesh mesh, string textAlignment){ 
        if(!string.IsNullOrEmpty(textAlignment)){
            TextAlignment alignment = TextAlignment.Center;
            if(textAlignment.Equals("right")){
                alignment = TextAlignment.Right;
            }else if(textAlignment.Equals("left")){
                alignment = TextAlignment.Left;
            }else if(textAlignment.Equals("center")){
                alignment = TextAlignment.Center;
            }
            if(mesh.alignment != alignment){
                mesh.alignment = alignment;
            }
        }
    }

    public static void UpdateFontStyle(TextMesh mesh, string textStyle){
        if(!string.IsNullOrEmpty(textStyle)){
            FontStyle fontStyle = FontStyle.Normal;
            if(textStyle.Equals("normal")){
                fontStyle = FontStyle.Normal;
            }else if(textStyle.Equals("bold")){
                fontStyle = FontStyle.Bold;
            }else if(textStyle.Equals("italic")){
                fontStyle = FontStyle.Italic;
            }else if(textStyle.Contains("bold") && 
                        textStyle.Contains("italic")){
                fontStyle = FontStyle.BoldAndItalic;
            }
            if(mesh.fontStyle != fontStyle){ 
                mesh.fontStyle = fontStyle;
            }
        }
    }

    public static void UpdateFontType(TextMesh mesh, string typeName, EHS_SceneManager sceneManager){
        // check if already set
        try{
            foreach(var name in mesh.font.fontNames){
                if(name.ToLower().Equals(typeName.ToLower())){ 
                    // font already set
                    return;
                }
            }
            var font = sceneManager.GetFont(typeName);
            if(font != null){
                var renderer = mesh.gameObject.GetComponent<Renderer>();
                var material = font.material;
                mesh.font = font;
                renderer.material = material;
            }
        }catch(Exception e){ 
            Debug.LogException(e);
        }
    }

    public static void UpdateText(TextMesh mesh, string text, bool clip=false){
        mesh.text = text;
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, GameObject unityObject, Color color){ 
        UpdateForegroundColor(ehsObject, unityObject, color, "");
    }

    public static void UpdateForegroundColor(EHS_Object ehsObject, Color color){ 
        UpdateForegroundColor(ehsObject, ehsObject.GetGameObject(), color);
    }

    /* Object Text Details Handler */
    public class TextDetailHandler : ObjectDetailHandler
    {
        public static readonly string TextValue = "textvalue";
        public static readonly string TextColor = "textcolor";

        public TextDetailHandler(EHS_SceneManager manager) : base(manager)
        {
            // register all material handlers
            // (1) text value
            objectDetailValuesHandlers.Add(TextValue, new TextDetailTextValueHandler(manager));
            // (2) text color
            objectDetailValuesHandlers.Add(TextColor, new TextDetailTextColorValueHandler(manager));
            // (3) ...
        }

        public abstract class TextDetailValueHandler : IObjectDetailValuesHandler
        {
            protected EHS_SceneManager manager;

            public TextDetailValueHandler(EHS_SceneManager manager)
            {
                this.manager = manager;
            }

            protected abstract bool UpdateText(EHS_Object ehsObject, GameObject unityObject, EHS_ConfigList configs);

            public bool Update(EHS_Object ehsObject, string type, string id, EHS_ConfigList configs) {
                try{
                    return UpdateText(ehsObject, ehsObject.GetGameObject(), configs);
                }catch(Exception e){ 
                    Debug.LogException(e);    
                }
                return false;
            }
        }

        public class TextDetailTextValueHandler : TextDetailValueHandler
        {
            public TextDetailTextValueHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateText(EHS_Object ehsObject, GameObject unityObject, EHS_ConfigList configs){ 
                try{
                    string text = "@TODO"; // @TODO if needed
                    var mesh = unityObject.GetComponent<TextMesh>();
                    float size = 1f; // @TODO if needed
                    mesh.fontSize = (int)(size*DEFAULT_FONT_SIZE);
                    mesh.text = EHS_Utils.FixText(text);
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }

        public class TextDetailTextColorValueHandler : TextDetailValueHandler
        {
            public TextDetailTextColorValueHandler(EHS_SceneManager manager) : base(manager)
            {
            }

            protected override bool UpdateText(EHS_Object ehsObject, GameObject unityObject, EHS_ConfigList configs){ 
                try{
                    string color = ""; // @TODO if needed
                    if(!string.IsNullOrEmpty(color)){
                        UpdateForegroundColor(ehsObject, unityObject, EHS_Utils.PareseColor(color, Color.white));
                        return true;
                    }else{
                        Debug.Log("Color value is not specified");
                    }
                    return true;
                }catch(Exception e){
                    Debug.LogException(e);
                }
                return false;
            }
        }
    }
}
