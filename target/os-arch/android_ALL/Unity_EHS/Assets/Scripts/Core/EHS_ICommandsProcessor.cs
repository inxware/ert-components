
public interface EHS_ICommandsProcessor
{
    void Start();
    void Read(EHS_FunctionBlockManager manager);
    void Write(EHS_FBResponse response);
}
