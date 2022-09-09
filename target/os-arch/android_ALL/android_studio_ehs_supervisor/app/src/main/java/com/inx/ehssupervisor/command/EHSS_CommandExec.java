package com.inx.ehssupervisor.command;
import androidx.annotation.NonNull;

import com.inx.ehssupervisor.configuration.EHSS_Constants;
import com.inx.ehssupervisor.utils.EHSS_Logger;
import com.inx.ehssupervisor.utils.EHSS_Utils;

import java.util.HashMap;

public class EHSS_CommandExec {

    private static final int COMMAND_DELAY = EHSS_Constants.COMMNAD_EXEC_TIME_INTERVAL;

    private final HashMap<String, EHSS_TimedCommand> commands;

    public EHSS_CommandExec(){
        this.commands = new HashMap<>();
    }

    public void register(@NonNull ICommand command, int interval){
        String id = command.id();
        if(!commands.containsKey(id)){
            commands.put(id, new EHSS_TimedCommand(command, interval));
        }else{
            EHSS_Logger.debug("Command already registered.");
        }
    }

    public void unregister(String id){
        // @TODO
    }

    public void start(){
        for (HashMap.Entry<String, EHSS_TimedCommand> entry : commands.entrySet()) {
            String id = entry.getKey();
            EHSS_Logger.debug("Command Exec starting command ("+id+").");
            EHSS_TimedCommand command = entry.getValue();
            if(command != null){
                command.start();
            }
        }
    }

    public void stop(){
        for (HashMap.Entry<String, EHSS_TimedCommand> entry : commands.entrySet()) {
            String id = entry.getKey();
            EHSS_Logger.debug("Command Exec stopping command ("+id+").");
            EHSS_TimedCommand command = entry.getValue();
            if(command != null){
                command.stop();
            }
        }
    }

    public void exec(){
        for (HashMap.Entry<String, EHSS_TimedCommand> entry : commands.entrySet()) {
            String id = entry.getKey();
            EHSS_TimedCommand command = entry.getValue();
            if(command != null && command.isReady()){
                EHSS_Logger.debug("Command Exec executing command ("+id+").");
                command.exec();
                command.restart();
            }
            EHSS_Utils.sleep(COMMAND_DELAY);
        }
    }
}
