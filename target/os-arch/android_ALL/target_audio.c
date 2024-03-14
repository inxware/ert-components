#include "target.h"
#include "target_audio.h"

#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
#include "target_audio_input_level_common.h"
#endif

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <stdio.h>
#include <math.h>

#include <android/log.h>
#if 1
#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "target_audio", __VA_ARGS__)
#else 
#define printf(...)
#endif

#define RECORD_DEVICE_KICKSTART_BUF_COUNT 2
#define DEVICE_SHADOW_BUFFER_QUEUE_LEN 1
#define BUF_COUNT 16

#define AUDIO_INPUT_BIAS_CONSTANT 0//9

#define MAX_NUMBER_INTERFACES       5
#define MAX_NUMBER_INPUT_DEVICES    3
#define POSITION_UPDATE_PERIOD      1000

#define BUFFER_SIZE_IN_SAMPLES  4096
#define BUFFER_SIZE_IN_BYTES    (1 * BUFFER_SIZE_IN_SAMPLES)

#define CheckErr(x) { \
SLresult res = x; \
if ( res != SL_RESULT_SUCCESS ) {\
    printf("ERROR checking RESULT as %d\n", res);\
    return 1;\
}}

SLObjectItf sl;

SLObjectItf recorder;

static SLint32 gMean = 0;

static SLint8 buf[BUFFER_SIZE_IN_BYTES * DEVICE_SHADOW_BUFFER_QUEUE_LEN];

static SLboolean targetAudioInitialised = SL_BOOLEAN_FALSE;

static SLint32 gPeriodCount = 1;
static SLint32 gPeriodCountMax = 1;

typedef struct CallbackContext_ {
    SLuint32    size;
    SLint8 *    pDataBase;  // Base address of local audio data storage
    SLint8 *    pData;      // Current address of local audio data storage
} CallbackContext;
    CallbackContext cntxt;

void targetRecordCallback(SLRecordItf rec, void *pContext, SLuint32 event)
{
    if (SL_RECORDEVENT_HEADATNEWPOS & event)
    {
        SLmillisecond pMsec = 0;
        (*rec)->GetPosition(rec, &pMsec);
        printf("SL_RECORDEVENT_HEADATNEWPOS current position = %lu ms\n", pMsec);
    }

    if (SL_RECORDEVENT_HEADATMARKER & event)
    {
        SLmillisecond pMsec = 0;
        (*rec)->GetPosition(rec, &pMsec);
        printf("SL_RECORDEVENT_HEADATMARKER current position = %lu ms\n", pMsec);
    }
}

void targetRecorderBufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void *rec)
{
    SLresult res;
    CallbackContext *pCntxt = (CallbackContext *)rec;
    // printf("\tpCntxt->pData: %p, pCntxt->pDataBase: %p, pCntxt->size: %lu\n", 
    //        pCntxt->pData, pCntxt->pDataBase, pCntxt->size);

    SLint32 number;
    SLint8 bufNum;
    SLint32 sum = 0;
    SLint32 mean = 0;
    SLint8 rightBuf[BUFFER_SIZE_IN_BYTES];
    SLint8 filterred_buffer_high[BUFFER_SIZE_IN_BYTES];
    SLint8 filterred_buffer_low[BUFFER_SIZE_IN_BYTES];

    int i;

    for (i = 0; i < BUFFER_SIZE_IN_BYTES; i++)
    {
        rightBuf[i] = (pCntxt->pData[i] << 4 & 0b11110000) | (pCntxt->pData[i] >> 4 & 0b1111);
    }

    filterred_buffer_high[0] = rightBuf[0];
    filterred_buffer_low[0] = rightBuf[0];

    for (i = 1; i < BUFFER_SIZE_IN_BYTES; i++)
    {
        filterred_buffer_high[i] = filterred_buffer_high[i - 1] + rightBuf[i] - rightBuf[i - 1];
    }

    for (i = 1; i < BUFFER_SIZE_IN_BYTES; i++)
    {
        filterred_buffer_low[i] = filterred_buffer_low[i - 1] + 7 * (filterred_buffer_high[i] - filterred_buffer_low[i - 1]) / 10;
    }

    for (i = 0; i < BUFFER_SIZE_IN_BYTES; i++) 
    {
        bufNum = filterred_buffer_low[i];//pCntxt->pData[i];
        //bufNum = (bufNum << 4 & 0b11110000) | (bufNum >> 4 & 0b1111);
        bufNum = bufNum > 0 ? bufNum : -bufNum;
        number = (SLint32) bufNum;
        sum += number;
    }

    mean = ((sum) / BUFFER_SIZE_IN_BYTES);

    if (gPeriodCount <= gPeriodCountMax)
    {
        gMean += mean;
        gMean /= 2;
    }
    else if (gPeriodCount > gPeriodCountMax)
    {
        gMean = gMean < AUDIO_INPUT_BIAS_CONSTANT ? 0 : (gMean - AUDIO_INPUT_BIAS_CONSTANT);
        printf("The average is %d\n", gMean);
#ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
        onCommonRecordFull(gMean);
#endif
        gMean = mean;
        gPeriodCount = 1;
        FILE *f = fopen("/sdcard/waveform.txt", "wb");
        fwrite(rightBuf, BUFFER_SIZE_IN_BYTES, sizeof(SLint8), f);
        fclose(f);
    }
    gPeriodCount++;

    pCntxt->pData += BUFFER_SIZE_IN_BYTES;

    if (pCntxt->pData >= pCntxt->pDataBase + (BUFFER_SIZE_IN_BYTES * DEVICE_SHADOW_BUFFER_QUEUE_LEN))
    {
        pCntxt->pData = pCntxt->pDataBase;
    }
    
    res = (*bq)->Enqueue(bq, buf, BUFFER_SIZE_IN_BYTES);
    if (res != SL_RESULT_SUCCESS) 
    {
        printf("Callback Failed\n");
        return;
    }

    SLAndroidSimpleBufferQueueState recQueueState;
    res = (*bq)->GetState(bq, &recQueueState);
    if (res != SL_RESULT_SUCCESS)
    {
        printf("Callback queue State Failed!\n");
        return;
    }
    // printf("\tRecBufferQueueCallback now has pCntxt->pData=%p queue: "
    //         "count=%lu playIndex=%lu\n",
    //         pCntxt->pData, recQueueState.count, recQueueState.index);
}

static SLint32 roundi(long long int dividend, long long int divisor)
{
    if (divisor == 0)
    {
        printf("Error: divided by 0.\n");
        return 0;
    }
    long long int temp_dividend = dividend * 10;
    long long int temp_result = temp_dividend / divisor;
    long long int result;
    if (temp_result >= 0) result = (temp_result + 5) / 10;
    else result = (temp_result - 5) / 10;

    return (SLint32)result;
}

ehs_uint8 targetAudioRecord_start( ehs_sint32 samplingRate, ehs_sint32 bitRate, ehs_sint32 period )
{
    printf("target Audio function Start\n");
    printf("sampling frequency: %d Hz, bit rate: %d, period: %d ms\n", samplingRate, bitRate, period);
    SLresult res;

    gPeriodCountMax = roundi(((long long int)period * (long long int)samplingRate * (long long int)bitRate) , ((long long int)8 * (long long int)1000 * (long long int)BUFFER_SIZE_IN_SAMPLES));
    printf("period is %ld ms, gPeriodCountMax is %d\n", period, gPeriodCountMax);
    gPeriodCountMax = gPeriodCountMax <= 0 ? 2 : gPeriodCountMax;
    gPeriodCount = 1;
    
    SLRecordItf recordItf;
    SLEngineItf EngineItf;

    SLObjectItf outputMix;
    SLEqualizerItf  equalizerItf;

    SLAndroidSimpleBufferQueueItf   RecordBufferQueue;

    SLAudioIODeviceCapabilitiesItf  AudioIODeviceCapabilitiesItf;
    SLAudioInputDescriptor          AudioInputDescriptor;

    SLDataSource            audioSource;
    SLDataLocator_IODevice  locator_mic;
    SLDeviceVolumeItf deviceVolumeItf;

    SLDataSink           audioSink;
    SLDataLocator_URI   uri;
    SLDataFormat_MIME   mime;
    SLDataLocator_AndroidSimpleBufferQueue loc_bq;
    SLAndroidDataFormat_PCM_EX format_pcm;

    int i;

    SLboolean       required[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    SLInterfaceID   iidArray[2] = {
        SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
        SL_IID_ANDROIDCONFIGURATION
    };

    SLuint32    InputDeviceIDs[MAX_NUMBER_INPUT_DEVICES];
    SLint32     numInputs = 0;
    SLboolean   mic_available = SL_BOOLEAN_FALSE;
    SLuint32    mic_deviceID = SL_DEFAULTDEVICEID_AUDIOINPUT;

    SLAndroidConfigurationItf inputConfig;

    // Create OpenSL ES Engine in thread-safe mode
    SLEngineOption EngineOption[] = {
        (SLuint32) SL_ENGINEOPTION_THREADSAFE,
        (SLuint32) SL_BOOLEAN_TRUE
    };

    //res = slCreateEngine(&sl, 1, EngineOption, 0, NULL, NULL);
    res = slCreateEngine(&sl, 0, NULL, 0, NULL, NULL);
    CheckErr(res);
    printf("Engine Created\n");

    // Realising the SL Engine in asynchronous mode
    res = (*sl)->Realize(sl, SL_BOOLEAN_FALSE);
    CheckErr(res);
    printf("SL Engine Realised\n");

    // Get the implicit SL Engine Interface
    res = (*sl)->GetInterface(sl, SL_IID_ENGINE, (void *)&EngineItf);
    CheckErr(res);
    printf("SL Engine Interface got\n");

    // Setup the data source structure
    locator_mic.locatorType = SL_DATALOCATOR_IODEVICE;
    locator_mic.deviceType  = SL_IODEVICE_AUDIOINPUT;
    locator_mic.deviceID    = SL_DEFAULTDEVICEID_AUDIOINPUT;
    locator_mic.device      = NULL;
    audioSource.pLocator    = (void *)&locator_mic;
    audioSource.pFormat     = NULL;

    // Setup the data sink structure
    loc_bq.locatorType  = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    loc_bq.numBuffers   = DEVICE_SHADOW_BUFFER_QUEUE_LEN;
    format_pcm.formatType       = SL_DATAFORMAT_PCM;
    format_pcm.numChannels      = 2;
    format_pcm.channelMask      = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    format_pcm.sampleRate       = (SLuint32) samplingRate * 1000;
    format_pcm.endianness       = SL_BYTEORDER_LITTLEENDIAN;
    format_pcm.bitsPerSample    = (SLuint32) bitRate;
    format_pcm.containerSize    = (SLuint32) bitRate;
    audioSink.pLocator  = (void *)&loc_bq;
    audioSink.pFormat   = (void *)&format_pcm;

    // Create audio recorder
    res = (*EngineItf)->CreateAudioRecorder(EngineItf, &recorder, &audioSource, &audioSink,
                                            sizeof(iidArray) / sizeof(iidArray[0]), iidArray, required);
    CheckErr(res);
    printf("Audio Recorder Instance Created\n");

    // Configure the voice recognition preset which has no signal processing for low latency
    res = (*recorder)->GetInterface(recorder, SL_IID_ANDROIDCONFIGURATION, &inputConfig);
    CheckErr(res);
    printf("Recorder get interface Got\n");

    SLuint32 presetValue = SL_ANDROID_RECORDING_PRESET_UNPROCESSED;//SL_ANDROID_RECORDING_PRESET_VOICE_RECOGNITION;
    res = (*inputConfig)->SetConfiguration(inputConfig, SL_ANDROID_KEY_RECORDING_PRESET, &presetValue, sizeof(SLuint32));
    CheckErr(res);
    printf("Audio Recorder configured\n");

    SLuint32 presetValueRead = SL_ANDROID_RECORDING_PRESET_NONE;
    SLuint32 presetSize = 2 * sizeof(SLuint32);
    res = (*inputConfig)->GetConfiguration(inputConfig, SL_ANDROID_KEY_RECORDING_PRESET, &presetSize, (void *)&presetValueRead);
    CheckErr(res);
    if (presetValueRead != presetValue)
    {
        printf("Audio Recorder is not configured properly!\n");
        return 1;
    }
    printf("Audio Recorder is confirmed to be configured properly!\n");

    // Realising the recorder in synchronous mode
    res = (*recorder)->Realize(recorder, SL_BOOLEAN_FALSE);
    CheckErr(res);
    printf("Audio Recorder realised\n");

    // Get the RECORD interface implicit
    res = (*recorder)->GetInterface(recorder, SL_IID_RECORD, (void *)&recordItf);
    CheckErr(res);
    printf("Audio Recorder Interface Got\n");

    // Get the Android Simple Queue interface
    res = (*recorder)->GetInterface(recorder, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &RecordBufferQueue);
    CheckErr(res);
    printf("Audio Simple Queue Interface Got\n");

    cntxt.pDataBase = (int8_t*) &buf;
    cntxt.pData = cntxt.pDataBase;
    cntxt.size = sizeof(buf);
    // Register Buffer Queue callback to handle the recorded audio data
    res = (*RecordBufferQueue)->RegisterCallback(RecordBufferQueue, targetRecorderBufferQueueCallback, &cntxt);
    CheckErr(res);
    printf("Audio Recorder Callback Registered\n");

    // Stop the recording in case it is recording to clear the queue
    res = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
    CheckErr(res);
    printf("Set the Record State to STOPPED\n");

    // Clear the Buffer Queue
    res = (*RecordBufferQueue)->Clear(RecordBufferQueue);
    CheckErr(res);
    printf("Cleared the Record Buffer Queue\n");

    // Enqueue the audio stream into the buffer
    for (i = 0 ; i < DEVICE_SHADOW_BUFFER_QUEUE_LEN ; i++)
    {
        res = (*RecordBufferQueue)->Enqueue(RecordBufferQueue, cntxt.pData, BUFFER_SIZE_IN_BYTES);
        CheckErr(res);
        cntxt.pData += BUFFER_SIZE_IN_BYTES;
    }
    cntxt.pData = cntxt.pDataBase;
    printf("Enqueue the buffer\n");

    // Record the Audio
    res = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);
    CheckErr(res);
    printf("Start Recording...\n");

    targetAudioInitialised = SL_BOOLEAN_TRUE;
    printf("target_audio function ends\n");
    return 0;
}

void targetAudioRecord_destroy( void )
{
    if (targetAudioInitialised != SL_BOOLEAN_TRUE)
    {
        printf("targetAudioRecord_destroy failed. The instances are not initialised\n");
        return;
    }
    // Destroy the recorder object
    (*recorder)->Destroy(recorder);

    // Shutdown OpenSL ES (SL)
    (*sl)->Destroy(sl);
}

