package com.inx.ehssupervisor.apps.ehs;

import android.content.Context;
import androidx.annotation.NonNull;

import com.inx.ehssupervisor.configuration.EHSS_Constants;
import com.inx.ehssupervisor.launcher.EHSS_Launcher;

public class EHSS_EHSLauncher extends EHSS_Launcher {

    public static final String NAME = EHSS_Constants.EHS_APP;
    public static final String PACKAGE_NAME = "com.inx.ehs";

    public EHSS_EHSLauncher(@NonNull Context context) {
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
