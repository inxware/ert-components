
public interface ICommand
{
    string Type();
    bool Exec();
}

public abstract class Command : ICommand
{
    protected EHS_SceneManager manager;

    public Command(EHS_SceneManager manager)
    {
        this.manager = manager;
    }

    public abstract string Type();
    public abstract bool Exec();
}
