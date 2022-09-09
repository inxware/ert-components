
public class EHS_SafeQueue<T>
{
    private EHS_Queue<T> queue = new EHS_Queue<T>();

    public void Push(T item)
    {
        lock (queue) {
            queue.Push(item);
        }
    }

    public T Grab()
    {
        lock (queue) {
            return queue.Grab();
        }
    }

    public bool Replace(int index, T item)
    { 
        lock (queue) {
            return queue.Replace(index, item);
        }
    }

    public T GetAt(int index){
        lock (queue) {
            return queue.GetAt(index);
        }
    }

    public T GetFirst(){
        return GetAt(0);
    }

    public void Clear()
    {
        lock (queue) {
            queue.Clear();
        }
    }

    public int Size(){
        lock (queue) {
            return queue.Size();
        }
    }

    public bool Ready()
    {
        lock (queue) {
            return queue.Ready();
        }
    }

}
