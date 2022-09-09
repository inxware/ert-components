using System.Threading; 

public abstract class EHS_Runnable
{
    private readonly object sync = new object();
    protected bool running = false;
    private Thread thread;

    public static void Sleep(int milliseconds){ 
        Thread.Sleep(milliseconds);    
    }

    public EHS_Runnable(){
        thread = new Thread(ThreadedWork);
    }

    private void ThreadedWork(){
        OnRunStart();
        while(running){ 
            Run();
        }
        OnRunStop();
    }

    public void Notify(){ 
        lock (sync){
            Monitor.Pulse(sync);
        }
    }

    protected void Lock(){ 
        lock (sync){
            Monitor.Wait(sync);
        }
    }

    public void Start(){
        if(!running){
            running = true;
            OnStart();
            thread.Start();
        }
    }

    public void Stop(){
        if(running){
            running = false;
            Notify();
            thread.Join();
            OnStop();
        }
    }

    protected virtual void OnStart(){
    }

    protected virtual void OnStop(){   
    }

    protected virtual void OnRunStart(){
    }

    protected virtual void OnRunStop(){   
    }

    protected abstract void Run();
}
