package com.inx.ehssupervisor.utils.comms;

import java.util.HashMap;

public interface IMessage {
    boolean isValid();
    Integer getType();
    String getString();
    HashMap<String, String> getProperties();
}