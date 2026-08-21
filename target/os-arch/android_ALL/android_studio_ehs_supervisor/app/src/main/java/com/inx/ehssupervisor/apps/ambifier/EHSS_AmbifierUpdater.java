package com.inx.ehssupervisor.apps.ambifier;

import android.content.Context;

import androidx.annotation.NonNull;

import com.inx.ehssupervisor.configuration.EHSS_Constants;
import com.inx.ehssupervisor.updater.EHSS_Updater;

public class EHSS_AmbifierUpdater extends EHSS_Updater {

    public static final String NAME = EHSS_Constants.AMBIFIER_APP;

    public EHSS_AmbifierUpdater(@NonNull Context context, @NonNull String address) {
        super(context, address);
    }

    @Override
    public String name() {
        return NAME;
    }

    @Override
    public String packageName() {
        return EHSS_AmbifierLauncher.PACKAGE_NAME;
    }
}