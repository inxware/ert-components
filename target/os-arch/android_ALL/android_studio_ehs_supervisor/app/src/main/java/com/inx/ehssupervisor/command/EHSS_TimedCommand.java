package com.inx.ehssupervisor.command;

import androidx.annotation.NonNull;
import com.inx.ehssupervisor.utils.EHSS_Timeout;

public class EHSS_TimedCommand extends EHSS_Timeout {

    private final ICommand command;

    public EHSS_TimedCommand(@NonNull ICommand command, int interval){
        super(interval);
        this.command = command;
    }

    public boolean isReady(){
        return isTimeout();
    }

    public boolean exec(){
        return command.exec();
    }
}
