
public class EHS_FBResponse
{
    private string id;
    private string message;

    public EHS_FBResponse(string id, string message)
    {
        this.id = id;
        this.message = message;
    }

    public string GetId()
    {
        return id;
    }

    public string GetMessage()
    {
        return message;
    }
}
