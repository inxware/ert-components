using UnityEngine;
using TMPro;

public class EHS_TextObjectFactory
{
    public static ITextObjectHandler Create(EHS_TextObjectConfig objectConfig){
        //return CreateTextMesh(objectConfig);
        return CreateTextMeshPro(objectConfig);
    }

    private static ITextObjectHandler CreateTextMesh(EHS_TextObjectConfig objectConfig){ 
        var mesh = objectConfig.parentObject.GetComponentInChildren<TextMesh>();
        if(mesh == null){
            if(objectConfig.onlyExisting){ 
                return null;    
            }
            GameObject textObject = GameObject.Instantiate(objectConfig.textPrefab, objectConfig.parentObject.transform);
            mesh = EHS_TextMeshHandler.GetTextMesh(objectConfig.parentObject, textObject, 5);
        }
        return new EHS_TextMeshHandler(mesh).Init(objectConfig);
    }

    private static ITextObjectHandler CreateTextMeshPro(EHS_TextObjectConfig objectConfig){ 
        var mesh = objectConfig.parentObject.GetComponentInChildren<TextMeshPro>();
        if(mesh == null){
            if(objectConfig.onlyExisting){ 
                return null;    
            }
            GameObject textObject = GameObject.Instantiate(objectConfig.textPrefab, objectConfig.parentObject.transform);
            mesh = EHS_TextMeshProHandler.GetTextMesh(objectConfig.parentObject, textObject);
        }
        return new EHS_TextMeshProHandler(mesh).Init(objectConfig);
    }
}
