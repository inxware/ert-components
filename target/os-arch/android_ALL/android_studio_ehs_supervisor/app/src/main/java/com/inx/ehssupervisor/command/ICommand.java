package com.inx.ehssupervisor.command;

import com.inx.ehssupervisor.configuration.IConfiguration;

public interface ICommand {
    void configure(IConfiguration configuration);
    boolean exec();
    String id();
}
