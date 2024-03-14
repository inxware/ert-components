

public class EHS_Timeout : EHS_Timer
{
    private float timeout = 0f; 

    public void Restart(){ 
        Start();
    }

    public void SetTimeout(float timeout){
        lock(this){
            this.timeout = timeout;
        }
    }

    public bool Timeout(){
        return Elapsed() >= timeout;    
    }

    public bool Timeout(float timeout){ 
        return Elapsed() >= timeout;    
    }
}
