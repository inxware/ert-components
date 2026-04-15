package com.inx.ehs.media.interfaces;

import com.inx.ehs.media.av.AVData;

public interface IEhsMediaPlayer {
    boolean load(AVData loadData);
    boolean unload();
    void destroy();
    void play();
    void stop();
    void pause();
    void volume(Integer value);
    void time(Integer value);
    Integer volume();
    Integer length();
    Integer time();
    String type();
}
