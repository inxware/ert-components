package com.inx.ehssupervisor.launcher;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.apps.ambifier.EHSS_AmbifierLauncher;
import com.inx.ehssupervisor.apps.ehs.EHSS_EHSLauncher;
import com.inx.ehssupervisor.command.ICommand;
import com.inx.ehssupervisor.configuration.IConfiguration;
import com.inx.ehssupervisor.utils.EHSS_Logger;
import com.inx.ehssupervisor.utils.EHSS_Utils;

import java.util.HashMap;

public class EHSS_LaunchCommand implements ICommand {

    public static final String NAME = "launcher";

    private final Context context;
    private EHSS_LauncherConfiguration configuration;

    public EHSS_LaunchCommand(@NonNull Context context){
        this.context = context;
        this.configuration = null;
    }

    public EHSS_LaunchCommand(@NonNull Context context, @NonNull EHSS_LauncherConfiguration configuration){
        this(context);
        configure(configuration);
    }

    @Override
    public void configure(IConfiguration configuration) {
        this.configuration = (EHSS_LauncherConfiguration)configuration;
    }

    @Override
    public boolean exec() {
        if(configuration == null){
            EHSS_Logger.error("Launcher Command must be configured.");
            return false;
        }
        HashMap<String, EHSS_Launcher> launchers = getLaunchers();
        boolean success = true;
        String command = configuration.getCommand();
        for (String name : configuration.getProgramNames()) {
            if(!launchers.containsKey(name)){
                EHSS_Logger.error("Unknown launcher ("+name+")");
                return false;
            }
            EHSS_Launcher launcher = launchers.get(name);
            if(command.equals(EHSS_LauncherConfiguration.COMMAND_LAUNCH)){
                if(!launcher.isRunning()){
                    EHSS_Logger.info("Launching app ("+name+")");
                    success = launcher.launch();
                }
            }else if(command.equals(EHSS_LauncherConfiguration.COMMAND_FOCUS)){ // this should be blocked if ui opened
                EHSS_Logger.debug("Focusing app ("+name+")");
                success = launcher.focus();
            }else if(command.equals(EHSS_LauncherConfiguration.COMMAND_KILL)){
                if(launcher.isRunning()){
                    EHSS_Logger.info("Killing app ("+name+")");
                    success = launcher.kill();
                }else{
                    EHSS_Logger.warning("Cannot kill app ("+name+") as it's not running.");
                }
            }else if(command.equals(EHSS_LauncherConfiguration.COMMAND_RESTART)){
                EHSS_Logger.info("Restarting app ("+name+")");
                success = launcher.restart();
            }else{
                EHSS_Logger.error("Unknown launcher command ("+command+")");
            }
            if(success){
                // post exec delay
                Integer postDelay = configuration.getPostDelay(name);
                if(postDelay != null && postDelay > 0){
                    EHSS_Logger.debug("Post-"+command+" delay ("+postDelay+"[ms]) of ["+name+"].");
                    EHSS_Utils.sleep(postDelay);
                }
            }
        }
        return success;
    }

    public HashMap<String, EHSS_Launcher> getLaunchers(){
        if(configuration == null){
            return null;
        }
        // register app launchers
        HashMap<String, EHSS_Launcher> launchers = new HashMap<>();
        for(String name : configuration.getProgramNames()){
            // (1) EHS app launcher
            if(name != null && name.equals(EHSS_EHSLauncher.NAME)){
                launchers.put(EHSS_EHSLauncher.NAME, new EHSS_EHSLauncher(context));
            }
            // (2) Ambifier app launcher
            if(name != null && name.equals(EHSS_AmbifierLauncher.NAME)){
                launchers.put(EHSS_AmbifierLauncher.NAME, new EHSS_AmbifierLauncher(context));
            }
            // ...
        }
        return launchers;
    }

    @Override
    public String id() {
        return NAME;
    }
}
