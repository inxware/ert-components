using System;

public class EHS_Timer
{
    private long timer_start = 0;
    private bool started = false;


    public static long Timer(){
        return DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
    }

    public virtual void Start(){
        lock(this){
            timer_start = Timer();
            started = true;
        }
    }

    public virtual void Stop(){
        lock(this){
            started = false;
            timer_start = 0;
        }
    }

    public bool IsStrated(){
        return started;
    }

    public float Elapsed(){
        long timer = Timer();
        long elapsed =  timer - timer_start;
        return ((float)elapsed)/1000f;
    }
}
