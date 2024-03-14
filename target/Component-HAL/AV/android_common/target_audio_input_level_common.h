#ifndef _TARGET_AUDIO_INPUT_LEVEL_COMMON_H_
#define _TARGET_AUDIO_INPUT_LEVEL_COMMON_H_

/**
 * Input the data when every data retrieve callback is triggerred.
 *
 * The RMS value of the audio input will be input when the AudioRecorder's callback is triggerred.
 *
 * @param mean  The RMS value of the audio input to be passed.
 * */
void onCommonRecordFull( ehs_sint32 mean );

#endif
