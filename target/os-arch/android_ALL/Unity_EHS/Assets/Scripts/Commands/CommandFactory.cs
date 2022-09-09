using UnityEngine;

public class CommandFactory
{
    public static Command Create(EHS_SceneManager manager, string jsonCommand)
    {
        // parse the JSON command
        EHS_CommandConfig commandConfig = EHS_CommandConfig.CreateFromJSON(jsonCommand);
        return Create(manager, commandConfig);
    }

    public static Command Create(EHS_SceneManager manager, EHS_CommandConfig commandConfig)
    {
        if(commandConfig == null){ 
            return null;
        }
        // parse the JSON command
        string type = commandConfig.type;
        if(type == null){
            Debug.Log("Unknown JSON type: " + commandConfig.ToJSON());
            return null;
        }
        Command command = null;
        if(type == CreateObjectCommand.TypeName){
            command = new CreateObjectCommand(manager, commandConfig.objectConfig);
        }else if(type == UpdateObjectCommand.TypeName){
            command = new UpdateObjectCommand(manager, commandConfig.objectConfig);
        }else if(type == StopObjectCommand.TypeName){
            command = new StopObjectCommand(manager, commandConfig.objectConfig);
        }else if(type == RemoveObjectCommand.TypeName){
            command = new RemoveObjectCommand(manager, commandConfig.id);
        }else if(type == AttachEventCommand.TypeName){
            command = new AttachEventCommand(manager, commandConfig.eventConfig);
        }else if(type == RemoveEventCommand.TypeName){
            command = new RemoveEventCommand(manager, commandConfig.eventConfig);
        }else if(type == CreateMediaSourceCommand.TypeName){
            command = new CreateMediaSourceCommand(manager, commandConfig.mediaSourceConfig);
        }
        //Debug.LogWarning(commandConfig.ToJSON());
        return command;
    }
}
