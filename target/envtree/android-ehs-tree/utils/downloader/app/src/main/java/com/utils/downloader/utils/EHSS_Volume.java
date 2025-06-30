package com.utils.downloader.utils;

import android.content.Context;
import android.media.AudioManager;

public class EHSS_Volume {

    public static void setMusicVolume(Context context, int volumeLevel) {
        // Get the AudioManager system service
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);

        // Check the maximum volume level for the STREAM_MUSIC
        int maxVolume = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);

        // Clamp the volume level between 0 and maxVolume
        int volume = Math.min(volumeLevel, maxVolume);

        // Set the music volume
        audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, volume, 0); // No UI feedback
    }

    public static int getMusicVolume(Context context) {
        AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
    }
}
