using System.Collections;
using System.Collections.Generic;

public class EHS_Queue<T>
{
    private List<T> queue;

    public EHS_Queue()
    {
        queue = new List<T>();
    }

    public void Push(T item)
    {
        queue.Add(item);
    }

    public T Grab()
    {
        if(queue.Count > 0){
            T item = queue[0];
            queue.RemoveAt(0);
            return item;
        }
        return default(T);
    }

    public bool Replace(int index, T item)
    { 
        if(queue.Count > index){
            queue[index] = item;
            return true;
        }
        return false;
    }

    public T GetAt(int index){
        if(queue.Count > index){
            T item = queue[index];
            return item;
        }
        return default(T);
    }

    public T GetFirst(){
        return GetAt(0);
    }

    public void Clear()
    {
        queue.Clear();
    }

    public int Size(){ 
        return queue.Count;
    }

    public bool Ready()
    {
        return queue.Count > 0;
    }
}
