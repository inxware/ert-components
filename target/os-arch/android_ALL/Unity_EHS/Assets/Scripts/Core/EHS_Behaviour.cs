using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface IBehaviourListener
{
    void OnCompleted();
}

public abstract class EHS_Behaviour : MonoBehaviour
{
    private List<IBehaviourListener> listeners;
    private IBehaviourListener listener;
    protected EHS_EventConfig config;
    private float targetSpeed = 0f;

    public void AddListener(IBehaviourListener listener)
    {
        if(listener != null){
            if(listeners == null){ 
                listeners = new List<IBehaviourListener>();
            }
            this.listeners.Add(listener);
        }
    }

    public void SetListener(IBehaviourListener listener)
    {
        this.listener = listener;
    }

    protected virtual bool CallBehaviourCompleted()
    {
        if(listener != null){ 
            listener.OnCompleted(); 
        }
        if(listeners != null){
            foreach(var listener in listeners){
                listener.OnCompleted();
            }
        }
        return true;
    }

    public void Modify(EHS_EventConfig config)
    {
        OnModify();
        if(config.steps != 0f && config.speed > 0f){
            targetSpeed = config.speed;
            if(this.config != null){
                if(this.config.speed > config.speed && config.steps > 0f)
                {
                    config.steps = -1f * config.steps;
                }
                config.speed = this.config.speed;
            }else{
                config.speed = 0f;
            }
            this.config = config;
        }else{
            this.config = config;
        }
        OnModified();
    }

    public void Remove()
    {
        Debug.Log("Remove event (" + Type() + ")");
        OnRemove();
    }

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if(config != null && ((config.steps > 0f && config.speed < targetSpeed) ||
           (config.steps < 0f && config.speed > targetSpeed))){
            config.speed += config.steps;
        }
        Exec();
    }

    public abstract string Type();

    protected abstract void Exec();

    protected virtual void OnModify(){
        // Do nothing 
    }

    protected virtual void OnModified(){
        // Do nothing  
    }

    protected virtual void OnRemove(){
        Debug.Log("EHS behaviour OnRemove not implemented in ["+Type()+"].");    
    }
}
