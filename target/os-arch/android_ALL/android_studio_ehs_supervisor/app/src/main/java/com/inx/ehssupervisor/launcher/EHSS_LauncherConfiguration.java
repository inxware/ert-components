package com.inx.ehssupervisor.launcher;

import android.content.Intent;

import com.inx.ehssupervisor.configuration.IConfiguration;
import com.inx.ehssupervisor.utils.EHSS_Logger;

import java.util.ArrayList;
import java.util.HashMap;

public class EHSS_LauncherConfiguration implements IConfiguration {

    public static final String COMMAND_LAUNCH = "launch";
    public static final String COMMAND_FOCUS = "focus";
    public static final String COMMAND_KILL = "kill";
    public static final String COMMAND_RESTART = "restart";

    private String command = COMMAND_LAUNCH;
    private final ArrayList<String> programNames = new ArrayList<>();
    private final HashMap<String, Integer> postDelays = new HashMap<>();

    public EHSS_LauncherConfiguration setCommand(String command){
        this.command = command;
        return this;
    }

    public EHSS_LauncherConfiguration setPostDelay(String programName, Integer delay){
        if(programNames.contains(programName)) {
            postDelays.put(programName, delay);
        }else {
            EHSS_Logger.debug("Cannot set post delay for unknown program.");
        }
        return this;
    }

    public EHSS_LauncherConfiguration addProgramName(String name){
        programNames.add(name);
        return this;
    }

    public String getCommand(){
        return command;
    }

    public Integer getPostDelay(String programName){
        if(postDelays.containsKey(programName)) {
            return postDelays.get(programName);
        }
        return null;
    }

    public ArrayList<String> getProgramNames(){
        return programNames;
    }
}
