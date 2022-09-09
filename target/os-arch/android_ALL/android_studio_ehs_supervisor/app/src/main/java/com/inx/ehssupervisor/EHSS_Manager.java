package com.inx.ehssupervisor;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.apps.ambifier.EHSS_AmbifierLauncher;
import com.inx.ehssupervisor.apps.ehs.EHSS_EHSLauncher;
import com.inx.ehssupervisor.command.EHSS_CommandExec;
import com.inx.ehssupervisor.command.ICommand;
import com.inx.ehssupervisor.configuration.EHSS_Constants;
import com.inx.ehssupervisor.configuration.JSONConfiguration;
import com.inx.ehssupervisor.devman.EHSS_DevmanServerPoll;
import com.inx.ehssupervisor.launcher.EHSS_LaunchCommand;
import com.inx.ehssupervisor.launcher.EHSS_Launcher;
import com.inx.ehssupervisor.launcher.EHSS_LauncherConfiguration;
import com.inx.ehssupervisor.updater.EHSS_UpdateCommand;
import com.inx.ehssupervisor.utils.EHSS_Logger;
import com.inx.ehssupervisor.utils.EHSS_Utils;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

public class EHSS_Manager {

    private static EHSS_Manager s_instance = null;
    private final Context context;
    private EHSS_ActivityLauncher activityLauncher = null;
    private Boolean focusBlocked = false;

    private EHSS_Manager(@NonNull Context context) {
        this.context = context;
    }

    public static EHSS_Manager getInstance(){
        return s_instance;
    }

    public static boolean createInstance(@NonNull Context context){
        if(s_instance != null){
            return false;
        }
        s_instance = new EHSS_Manager(context);
        return true;
    }

    public static void destroyInstance(){
        if(s_instance != null){
            s_instance.destroy();
            s_instance = null;
        }
    }

    private EHSS_LauncherConfiguration launcherConfiguration(){
        return new EHSS_LauncherConfiguration()
                .setCommand(EHSS_LauncherConfiguration.COMMAND_LAUNCH)
                .addProgramName(EHSS_AmbifierLauncher.NAME)
                .addProgramName(EHSS_EHSLauncher.NAME);
    }

    private EHSS_LauncherConfiguration startLaunchConfiguration(){
        return new EHSS_LauncherConfiguration()
                .setCommand(EHSS_LauncherConfiguration.COMMAND_LAUNCH)
                .addProgramName(EHSS_AmbifierLauncher.NAME)
                .addProgramName(EHSS_EHSLauncher.NAME)
                .setPostDelay(EHSS_AmbifierLauncher.NAME, 8000);
    }

    private EHSS_LauncherConfiguration focusAppConfiguration(){
        return new EHSS_LauncherConfiguration()
                .setCommand(EHSS_LauncherConfiguration.COMMAND_FOCUS)
                .addProgramName(EHSS_AmbifierLauncher.NAME);
    }

    private void registerCommands(EHSS_CommandExec commandExec){
        // (1) register devman server poll
        commandExec.register(new EHSS_DevmanServerPoll(context), EHSS_Constants.DEVMAN_POLL_TIME_INTERVAL);
        // (2) register keep a live - @TODO
        //commandExec.register(new EHSS_LaunchCommand(context, launcherConfiguration()), EHSS_Constants.KEEP_ALIVE_TIME_INTERVAL);
        // (3) keep focused
        //commandExec.register(new EHSS_LaunchCommand(context, focusAppConfiguration()), EHSS_Constants.KEEP_FOCUSED_TIME_INTERVAL);
    }

    public void run(){
        // initialise
        boolean isInitialised = false;
        while(!isInitialised) {
            isInitialised = initialise();
        }

        // Create command exec
        final EHSS_CommandExec commandExec = new EHSS_CommandExec();
        registerCommands(commandExec);
        commandExec.start();

        // run until stopped @TODO - stopping
        boolean isRunning = true;
        while(isRunning){
            commandExec.exec();
        }
    }

    private boolean initialise(){
        // launch applications on startup
        EHSS_Logger.info("[Start] Initialise EHS Supervisor.");
        EHSS_LauncherConfiguration launcherConfiguration = startLaunchConfiguration();

        // create a list of start-up commands
        ArrayList<ICommand> startupCommands = new ArrayList<>();

        try {
            EHSS_LaunchCommand launchCommand = new EHSS_LaunchCommand(context, launcherConfiguration);
            // add apps to be installed for the first time
            for (HashMap.Entry<String, EHSS_Launcher> entry : launchCommand.getLaunchers().entrySet()) {
                EHSS_Launcher launcher = entry.getValue();
                // Check if the package is already installed
                if(!EHSS_Utils.isInstalled(context, launcher.packageName())){
                    JSONObject updateJson = EHSS_UpdateCommand.createRequest(launcher.name());
                    if(updateJson != null) {
                        EHSS_UpdateCommand updateCommand = new EHSS_UpdateCommand(context);
                        updateCommand.configure(new JSONConfiguration(updateJson));
                        // append command for installing
                        startupCommands.add(updateCommand);
                    }else {
                        throw new Exception("Invalid new install request json.");
                    }
                }
            }
            // add apps launcher
            startupCommands.add(launchCommand);
            // add app for focus
            startupCommands.add(new EHSS_LaunchCommand(context, focusAppConfiguration()));

            // run all start-up commands
            for(ICommand command : startupCommands){
                EHSS_Utils.sleep(EHSS_Constants.STARTUP_LAUNCH_DELAY);
                if(!command.exec()){
                    throw new Exception("Failed executing command ("+command.id()+").");
                }
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
            EHSS_Logger.error("[End] EHS Supervisor initialisation failed.");
            EHSS_Utils.sleep(EHSS_Constants.STARTUP_LAUNCH_DELAY);
            return false;
        }

        EHSS_Logger.info("[End] EHS Supervisor initialisation complete.");
        return true;
    }

    private void destroy(){
        // add any clean code
    }

    // This function is used for launching activities for result e.g. APK
    // installer from the foreground process.
    public boolean launchActivity(EHSS_ActivityLauncher launcher){
        if(launcher == null){
            EHSS_Logger.error("Activity launcher cannot be set as null.");
            return false;
        }
        if(activityLauncher != null){
            EHSS_Logger.error("Activity launcher already in use.");
            return false;
        }
        EHSS_Logger.error("Launching UI activity as part of activity launching.");
        blockAppFocus(true);
        activityLauncher = launcher;
        activityLauncher.launch();
        activityLauncher.waitLock(); // wait until signaled
        activityLauncher.setValid(false);
        activityLauncher = null;
        blockAppFocus(false);
        return true;
    }

    public EHSS_ActivityLauncher getActivityLauncher(){
        return activityLauncher;
    }

    public synchronized boolean isAppFocusBlocked(){
        return focusBlocked;
    }

    public synchronized void blockAppFocus(boolean focusBlocked){
        EHSS_Logger.debug("Block application focus [" +focusBlocked+ "]");
        this.focusBlocked = focusBlocked;
    }
}
