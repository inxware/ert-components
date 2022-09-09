
using UnityEngine;

public class EHS_TextObjectConfig 
{
    public EHS_Object ehsObject;
    public GameObject textPrefab;
    public GameObject parentObject;
    public string side;
    public bool onlyExisting = false; // set to true to create handlers for only existing text objects 

    public EHS_TextObjectConfig SetEhsObject(EHS_Object ehsObject){
        this.ehsObject = ehsObject;
        return this;    
    }

    public EHS_TextObjectConfig SetTextPrefab(GameObject textPrefab){
        this.textPrefab = textPrefab;
        return this;    
    }

    public EHS_TextObjectConfig SetParentObject(GameObject parentObject){
        this.parentObject = parentObject;
        return this;    
    }

    public EHS_TextObjectConfig SetSide(string side){
        this.side = side;
        return this;    
    }

    public EHS_TextObjectConfig SetOnlyExisting(bool enable){
        this.onlyExisting = enable;
        return this;    
    }
}
