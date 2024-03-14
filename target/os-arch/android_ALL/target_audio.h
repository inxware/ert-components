#ifndef _TARGET_AUDIO_H_
#define _TARGET_AUDIO_H_

#include "globals.h"

/**
 * Start the Audio Record
 *
 * Start the Audio recording with given sampling rate in Hertz up to 192KHz, and bitRate.
 *
 * @see targetAudioRecord_destroy
 *
 * @param samplingRate      The sampling frequency in Hertz. Up to 192000.
 * @param bitRate           The bit rate of the sample. Up to 16.
 * @param period            The time interval of every RMS value in ms.
 *
 * @return 0 if the start is successful.
 * */
ehs_uint8 targetAudioRecord_start( ehs_sint32 samplingRate, ehs_sint32 bitRate, ehs_sint32 period );

/**
 * Destroy the Audio Record interface
 *
 * Stop and destroy all audio recording interfaces. This needs to be called before every new AudioRecord (OpenSL ES) related
 * object is created.
 *
 * @see targetAudioRecord_start
 * */
void targetAudioRecord_destroy( void );

#endif
