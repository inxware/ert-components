using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_EventHandler
{
    public interface IEventDetailValuesHandler
    {
        bool Attach(EHS_Object ehsObject, EHS_EventConfig config, string [] details);
    }

    private interface IEventHandler
    {
        bool Attach(EHS_Object ehsObject, EHS_EventConfig config);
        bool Attach(GameObject unityObject, EHS_EventConfig config);
        void Remove(EHS_Object ehsObject);
        void Remove(GameObject unityObject);
        EHS_Behaviour GetEventObject(EHS_Object ehsObject);
        EHS_Behaviour GetEventObject(GameObject unityObject);
        bool SetCallBack(EHS_Object ehsObject, IBehaviourListener listener);
        bool AddCallBack(EHS_Object ehsObject, IBehaviourListener listener);
    }

    public class DestroyOnCompleted : IBehaviourListener
    {
        private EHS_Object ehsObject;
        public DestroyOnCompleted(EHS_Object ehsObject)
        {
            this.ehsObject = ehsObject;
        }

        public void OnCompleted()
        {
            ehsObject.GetManager().DestroyObject(ehsObject.GetId());
        }
    }

    private class EventHandler<T> : IEventHandler where T : EHS_Behaviour
    {
        public bool Attach(EHS_Object ehsObject, EHS_EventConfig config)
        {
            if(ehsObject != null){ 
                return Attach(ehsObject.GetGameObject(), config);
            }
            return false;
        }

        public bool Attach(GameObject unityObject, EHS_EventConfig config)
        {
            T ehsEvent = unityObject.GetComponent<T>();
            if(ehsEvent != null){
                ehsEvent.Modify(config);
            }else{
                unityObject.AddComponent<T>().Modify(config);
            }
            return true;
        }

        public void Remove(EHS_Object ehsObject)
        {
            if(ehsObject != null){
                Remove(ehsObject.GetGameObject());    
            }
        }

        public void Remove(GameObject unityObject)
        {
            if(unityObject != null){
                T ehsEvent = unityObject.GetComponent<T>();
                if(ehsEvent != null){
                    ehsEvent.Remove();
                    GameObject.Destroy(ehsEvent);
                }
            }
        }

        public EHS_Behaviour GetEventObject(EHS_Object ehsObject)
        {
            if(ehsObject != null){
                return GetEventObject(ehsObject.GetGameObject());
            }
            return null;
        }

        public EHS_Behaviour GetEventObject(GameObject unityObject)
        {
            if(unityObject != null){
                return unityObject.GetComponent<T>();
            }
            return null;
        }

        public bool SetCallBack(EHS_Object ehsObject, IBehaviourListener listener)
        {
            try{
                var ehsEvent = GetEventObject(ehsObject);
                if(ehsEvent != null){
                    ehsEvent.SetListener(listener);
                    return true;
                }
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }

        public bool AddCallBack(EHS_Object ehsObject, IBehaviourListener listener){ 
            try{
                var ehsEvent = GetEventObject(ehsObject);
                if(ehsEvent != null){
                    ehsEvent.AddListener(listener);
                    return true;
                }
            }catch(Exception e){
                Debug.LogException(e);
            }
            return false;
        }
    }

    private static int DETAIL_VALUE_TYPE_INDEX = 0;

    private Dictionary<string, IEventHandler> eventHandlers;
    private Dictionary<string, IEventDetailValuesHandler> eventDetailValueHandler;

    public EHS_EventHandler(EHS_SceneManager manager)
    {
        eventHandlers = new Dictionary<string, IEventHandler>();
        eventDetailValueHandler = new Dictionary<string, IEventDetailValuesHandler>();

        // register event handlers
        eventHandlers.Add(EHS_Move.TypeName, new EventHandler<EHS_Move>());
        eventHandlers.Add(EHS_MoveTo.TypeName, new EventHandler<EHS_MoveTo>());
        eventHandlers.Add(EHS_MoveWithin.TypeName, new EventHandler<EHS_MoveWithin>());
        eventHandlers.Add(EHS_Rotate.TypeName, new EventHandler<EHS_Rotate>());
        eventHandlers.Add(EHS_RotateBy.TypeName, new EventHandler<EHS_RotateBy>());
        eventHandlers.Add(EHS_Scale.TypeName, new EventHandler<EHS_Scale>());

        // register event detail handlers
        eventDetailValueHandler.Add("hierarchy", new HierarchyObjectsEventAttach(this));
    }

    public bool AttachEvent(EHS_Object ehsObject, EHS_EventConfig config)
    {
        if(config != null){
            string [] details = config.details;
            if(details != null && details.Length > 0){
                string type = details[DETAIL_VALUE_TYPE_INDEX];
                if(eventDetailValueHandler.ContainsKey(type)){
                    IEventDetailValuesHandler handler = eventDetailValueHandler[type];
                    return (handler != null) && handler.Attach(ehsObject, config, details);
                }
            }else{
                return Attach(ehsObject, config);
            }
        }
        return false;
    }

    public bool Attach(EHS_Object ehsObject, EHS_EventConfig config)
    {
        if(config != null && eventHandlers.ContainsKey(config.type)){
            var handler = eventHandlers[config.type] as IEventHandler;
            if(handler != null && handler.Attach(ehsObject, config)){
                // Set callback for notify on complete
                if(config.notifyOnComplete){ 
                    var callback = ehsObject.GetBehaviourCallBack(config.type);
                    if(callback != null){ 
                        if(!handler.SetCallBack(ehsObject, callback)){
                            Debug.LogWarning("Failed to set on complete callback for object ("+ehsObject.GetId()+").");
                        }
                    }
                }
                // Check if destroy upon complete was requested
                // Needs to be added as a last one
                if(config.destroy){
                    if(!handler.AddCallBack(ehsObject, new DestroyOnCompleted(ehsObject))){
                        Debug.Log("Failed to set destroy event upon complete.");
                    }
                }
                return true;
            }
        }
        return false;
    }

    public bool Attach(GameObject unityObject, EHS_EventConfig config)
    {
        if(config != null && eventHandlers.ContainsKey(config.type)){
            var handler = eventHandlers[config.type] as IEventHandler;
            if(handler != null && handler.Attach(unityObject, config)){
                return true;
            }
        }
        return false;
    }

    public bool RemoveEvent(EHS_Object ehsObject, EHS_EventConfig config)
    {
        if(config != null && eventHandlers.ContainsKey(config.type)){
            var handler = eventHandlers[config.type] as IEventHandler;
            if(handler != null){
                handler.Remove(ehsObject);
                return true;
            }
        }
        return false;
    }

    public bool SetCallBack(EHS_Object ehsObject, string ehsEventID, IBehaviourListener listener)
    {
        if(string.IsNullOrEmpty(ehsEventID) && eventHandlers.ContainsKey(ehsEventID)){
            var handler = eventHandlers[ehsEventID] as IEventHandler;
            return (handler != null) && handler.SetCallBack(ehsObject, listener);
        }
        return false;
    }

    private class HierarchyObjectsEventAttach : IEventDetailValuesHandler
    {
        private static readonly int MAX_DETAILS = 2;
        private static readonly int DETAIL_VALUE_INDEX = 1;

        private EHS_EventHandler eventHandler;

        public HierarchyObjectsEventAttach(EHS_EventHandler eventHandler)
        {
            this.eventHandler = eventHandler;
        }

        public bool Attach(EHS_Object ehsObject, EHS_EventConfig config, string [] details)
        {
            if(details != null && details.Length == MAX_DETAILS){
                string value = details[DETAIL_VALUE_INDEX];
                if(value == "all"){
                    ChildrenOnly(ehsObject, config);
                    eventHandler.Attach(ehsObject, config);
                }else if(value == "onlyParent"){
                    eventHandler.Attach(ehsObject, config);
                }else if(value == "onlyChildren"){
                    ChildrenOnly(ehsObject, config);
                }else{
                    // handle specific child e.g. using its name
                    // invalid log
                    return false;
                }
                return true;
            }
            return false;
        }

        private void ChildrenOnly(EHS_Object ehsObject, EHS_EventConfig config)
        {
            GameObject unityObject = ehsObject.GetGameObject();
            Transform parentTransform = unityObject.transform;
            foreach(Transform childTransform in parentTransform){
                GameObject child = childTransform.gameObject;
                eventHandler.Attach(child, config);
            }
        }
    }
}
