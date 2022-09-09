package com.inx.ehssupervisor.apps.ambifier;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.configuration.EHSS_Constants;
import com.inx.ehssupervisor.launcher.EHSS_Launcher;

public class EHSS_AmbifierLauncher extends EHSS_Launcher {

    public static final String NAME = EHSS_Constants.AMBIFIER_APP;
    public static final String PACKAGE_NAME = "com.TheSoundAgency.Ambifier2";

    public EHSS_AmbifierLauncher(@NonNull Context context) {
        super(context);
    }

    @Override
    public String name() {
        return NAME;
    }

    @Override
    public String packageName() {
        return PACKAGE_NAME;
    }
}
