
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class EHS_TextMeshProHandler : ITextObjectHandler {

    private TextMeshPro textMesh;
    private EHS_TextAnimate textAnimate;
    private EHS_TextObjectConfig objectConfig;

    public EHS_TextMeshProHandler(TextMeshPro textMesh){ 
        if(textMesh == null){ 
            throw new Exception("Unable to create TextMeshPro object handler due to TextMeshPro object being null.");    
        }
        this.textMesh = textMesh;
        textAnimate = new EHS_TextAnimate(textMesh);
    }

    public ITextObjectHandler Init(EHS_TextObjectConfig objectConfig){ 
        if(objectConfig == null){ 
            throw new Exception("Unable to create TextMeshPro object handler due to EHS_TextObjectConfig object being null.");    
        }
        this.objectConfig = objectConfig;
        return this;
    }

    public ITextObjectHandler Update(EHS_TextConfig textConfig, string text) {
        if(textConfig == null){ 
            throw new Exception("Unable to create TextMesh object handler due to EHS_TextConfig object being null.");    
        }
        textAnimate.SetAnimation(textConfig.animationConfig);
        SetFontSize(textConfig.fontScale);
        SetMargin(textConfig.margin);
        SetWrapping(textConfig.enableWrapping);
        SetOverflow(textConfig.overflowMode);
        SetTextAlignment(textConfig.alignment);
        SetFontStyle(textConfig.fontStyle);
        SetFontType(textConfig.fontType);
        var color = textConfig.color;
        if(!string.IsNullOrEmpty(text) && !string.IsNullOrEmpty(color)){
            SetColor(EHS_Utils.PareseColor(color, Color.white));
        }
        //SetGlow(textConfig.glowConfig); - @TDOD - fix memory leak
        textMesh.UpdateMeshPadding();
        
        SetText(EHS_Utils.FixText(text));
        return this;
    }

    public ITextObjectHandler Clear() {
        SetText("");
        return this;
    }

    public void SetColor(Color color) {
        textMesh.color = color;
    }

    public void SetFontSize(float fontScale) {
        fontScale = (fontScale > 1f) ? 1f : fontScale;
        Vector3 unityObjectScale = textMesh.transform.localScale;
        float scale = 1f;
        if(unityObjectScale.x > unityObjectScale.y){ 
            scale = (unityObjectScale.x/unityObjectScale.y);         
        }
        float c = 13.8f * scale;
        fontScale *= c; 
        textMesh.fontSize = fontScale;      
    }

    public void SetMargin(float margin){ 
        if(textMesh.margin.x != margin){ 
            textMesh.margin = new Vector4(margin,margin,margin,margin);
        }
    }

    public void SetWrapping(bool enable){
        if(textMesh.enableWordWrapping != enable){
            textMesh.enableWordWrapping = enable;
        }
    }

    private static Dictionary<string, TextOverflowModes> textOverflowModesMap = new Dictionary<string, TextOverflowModes>{ 
        {"overflow", TextOverflowModes.Overflow},
        {"ellipsis", TextOverflowModes.Ellipsis},
        {"masking", TextOverflowModes.Masking},
        {"truncate", TextOverflowModes.Truncate},
        {"scrollrect", TextOverflowModes.ScrollRect},
        {"page", TextOverflowModes.Page},
        {"linked", TextOverflowModes.Linked}
    };

    public void SetOverflow(string overflowMode){
        if(!string.IsNullOrEmpty(overflowMode)){
            overflowMode = overflowMode.ToLower();
            if(textOverflowModesMap.ContainsKey(overflowMode)){ 
                TextOverflowModes mode = textOverflowModesMap[overflowMode];
                if(textMesh.overflowMode != mode){ 
                    textMesh.overflowMode = mode;
                }
            }
        }
    }

    private static Dictionary<string, FontStyles> fontStylesMap = new Dictionary<string, FontStyles>{ 
        {"normal", FontStyles.Normal},
        {"bold", FontStyles.Bold},
        {"italic", FontStyles.Italic},
        {"underline", FontStyles.Underline},
        {"lowercase", FontStyles.LowerCase},
        {"uppercase", FontStyles.UpperCase},
        {"smallcaps", FontStyles.SmallCaps},
        {"strikethrough", FontStyles.Strikethrough},
        {"superscript", FontStyles.Superscript},
        {"subscript", FontStyles.Subscript},
        {"highlight", FontStyles.Highlight}
    };

    public void SetFontStyle(string textStyle) {
        if(!string.IsNullOrEmpty(textStyle)){
            textStyle = textStyle.ToLower();
            FontStyles fontStyle = FontStyles.Normal;
            foreach(var styleItem in fontStylesMap){ 
                if(textStyle.Contains(styleItem.Key)){ 
                    fontStyle |= styleItem.Value;
                }
            }
            if(textMesh.fontStyle != fontStyle){ 
                textMesh.fontStyle = fontStyle;
            }
        }
    }

    private static Dictionary<string, TextAlignmentOptions> textAlignmentOptionsMap = new Dictionary<string, TextAlignmentOptions> {
        {"topleft", TextAlignmentOptions.TopLeft},
        {"top", TextAlignmentOptions.Top},
        {"topright", TextAlignmentOptions.TopRight},
        {"topjustified", TextAlignmentOptions.TopJustified},
        {"topflush", TextAlignmentOptions.TopFlush},
        {"topgeoaligned", TextAlignmentOptions.TopGeoAligned},
        {"left", TextAlignmentOptions.Left},
        {"center", TextAlignmentOptions.Center},
        {"right", TextAlignmentOptions.Right},
        {"justified", TextAlignmentOptions.Justified},
        {"flush", TextAlignmentOptions.Flush},
        {"centergeoaligned", TextAlignmentOptions.CenterGeoAligned},
        {"bottomleft", TextAlignmentOptions.BottomLeft},
        {"bottom", TextAlignmentOptions.Bottom},
        {"bottomright", TextAlignmentOptions.BottomRight},
        {"bottomjustified", TextAlignmentOptions.BottomJustified},
        {"bottomflush", TextAlignmentOptions.BottomFlush},
        {"bottomgeoaligned", TextAlignmentOptions.BottomGeoAligned},
        {"baselineleft", TextAlignmentOptions.BaselineLeft},
        {"baseline", TextAlignmentOptions.Baseline},
        {"baselineright", TextAlignmentOptions.BaselineRight},
        {"baselinejustified", TextAlignmentOptions.BaselineJustified},
        {"baselineflush", TextAlignmentOptions.BaselineFlush},
        {"baselinegeoaligned", TextAlignmentOptions.BaselineGeoAligned},
        {"midlineleft", TextAlignmentOptions.MidlineLeft},
        {"midline", TextAlignmentOptions.Midline},
        {"midlineright", TextAlignmentOptions.MidlineRight},
        {"midlinejustified", TextAlignmentOptions.MidlineJustified},
        {"midlineflush", TextAlignmentOptions.MidlineFlush},
        {"midlinegeoaligned", TextAlignmentOptions.MidlineGeoAligned},
        {"caplineleft", TextAlignmentOptions.CaplineLeft},
        {"capline", TextAlignmentOptions.Capline},
        {"caplineright", TextAlignmentOptions.CaplineRight},
        {"caplinejustified", TextAlignmentOptions.CaplineJustified},
        {"caplineflush", TextAlignmentOptions.CaplineFlush},
        {"caplinegeoaligned", TextAlignmentOptions.CaplineGeoAligned},
        {"converted", TextAlignmentOptions.Converted}
    };

    public void SetTextAlignment(string textAlignment) {
        if(!string.IsNullOrEmpty(textAlignment)){
            textAlignment = textAlignment.ToLower();
            if(textAlignmentOptionsMap.ContainsKey(textAlignment)){ 
                TextAlignmentOptions alignment = textAlignmentOptionsMap[textAlignment];
                if(textMesh.alignment != alignment){ 
                    textMesh.alignment = alignment;
                }
            }
        }
    }

    public void SetFontType(string typeName) {
        if(!string.IsNullOrEmpty(typeName)){
            // check if already set
            try{
                string name = typeName.ToLower();
                if(textMesh.font.faceInfo.familyName.ToLower().Equals(name)){ 
                    // font already set
                    return;
                }
                var sceneManager = objectConfig.ehsObject.GetManager();
                var font = sceneManager.GetProFont(name);
                if(font != null){
                    textMesh.font = font;
                }
            }catch(Exception e){ 
                Debug.LogException(e);
            }
        }
    }

    public void SetText(string text) {
        textAnimate.SetText(text);
    }

    private static Dictionary<string, int> glowMaterialIndexMap = new Dictionary<string, int> { 
        { "arial", 0 },
        { "c64 pro mono", 1 },
        { "times new roman", 2 }
    };

    private void SetGlow(EHS_GlowConfig config){
        try{
            Material fontMaterial = textMesh.fontMaterial;
            if(config.enabled){
                if(!fontMaterial.IsKeywordEnabled("GLOW_ON")){
                    string fontName = textMesh.font.faceInfo.familyName;
                    int index = glowMaterialIndexMap[fontName.ToLower()];
                    var sceneManager = objectConfig.ehsObject.GetManager();
                    textMesh.fontSharedMaterial = sceneManager.GetProFontGlowMaterial(index);
                }
                var color = EHS_Utils.PareseColor(config.color, Color.white);
                textMesh.fontMaterial.SetColor("_GlowColor", color);
                textMesh.fontMaterial.SetFloat(ShaderUtilities.ID_GlowOffset, config.offset);
                textMesh.fontMaterial.SetFloat(ShaderUtilities.ID_GlowInner, config.inner);
                textMesh.fontMaterial.SetFloat(ShaderUtilities.ID_GlowOuter, config.outer);
                textMesh.fontMaterial.SetFloat(ShaderUtilities.ID_GlowPower, config.power);
            }else{ 
                if(fontMaterial.IsKeywordEnabled("GLOW_ON")){
                    string fontName = textMesh.font.faceInfo.familyName;
                    var sceneManager = objectConfig.ehsObject.GetManager();
                    textMesh.fontSharedMaterial = sceneManager.GetProFont(fontName.ToLower()).material;
                }    
            }
        }catch(Exception e){ 
            Debug.LogException(e);    
        }
    }
      
    public static TextMeshPro GetTextMesh(GameObject unityObject, GameObject textObject){
        var scale = Vector3.one;
        var unityObjectScale = unityObject.transform.parent.localScale;
        if(unityObjectScale.x > unityObjectScale.y){ 
            scale.x *= (unityObjectScale.y/unityObjectScale.x);         
        }else{ 
            scale.y *= (unityObjectScale.x/unityObjectScale.y);    
        }
        textObject.transform.localScale = scale;
        textObject.transform.localPosition -= Vector3.forward*0.01f;
        var rect = textObject.GetComponent<RectTransform>();
        rect.sizeDelta = new Vector2(1f/scale.x, 1f/scale.y);
        return textObject.GetComponent<TextMeshPro>();
    }
}
