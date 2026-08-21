# -*- coding: utf-8 -*-
"""
Created on Thu Dec 20 11:44:34 2018

@author: inx limited
"""

##Necessary modules
import sounddevice as sd
import matplotlib.pyplot as plt
from scipy import signal
from scipy.io import wavfile
import numpy as np
import sys
import math
#import os
#sys.path.append(os.path.join(sys.path[0],'....', 'SystemTests', 'CI','projects', 'player'))
import pickle
import junit_xml as ju
import requests
import devman
import tests
import argparse
import time


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Run the set of release tests for a given device group')
    parser.add_argument('--device', help='ID of device that will play the test audio', required=True)
    parser.add_argument('--playlist', help='ID of playlist that contains the test audio', required=True)
    parser.add_argument('--jwt', help='JWT of user connected to test device', required=True)
    #parser.add_argument('--group', help='group name that you to test usually the TEST_NAME', required=True)
    args = parser.parse_args()
    #groupName=args.group
    deviceId=args.device
    playlistId=args.playlist
    jwt=args.jwt
    testSuites=[]
    testSuite1=tests.TestSuite()
    testSuites.append(testSuite1)
    testSuite1.name='Media Scheduler Tests'

    version = 'Windows'
    if version == 'Server':
        test_address = '/root/opt/test_models/Test.wav'
        model_address = '/root/opt/test_models/newestmodel.wav'
    elif version == 'patrick':
        test_address = './Test.wav'
        model_address = 'newmodel8k.wav'
    elif version == 'Windows':
        #test_address = '../../SystemTests/test-scripts/functional/playlist_scheduler/Recordandcompare/Test.wav'
        test_address = './Test-07-21.wav'
        #test_address = 'SystemTests/test-scripts/functional/playlist_scheduler/Recordandcompare/Test-07-21.wav'
        #test_address = 'SystemTests/projects/ambifier/Test-07-21.wav'
        #test_address = 'SystemTests/test-scripts/functional/playlist_scheduler/Recordandcompare/Test-Silence.wav'
        model_address = './newmodel8k_4_tones.wav'
        #model_address = '../../SystemTests/test-scripts/functional/playlist_scheduler/Recordandcompare/newmodel8k.wav'
        #model_address = 'SystemTests/test-scripts/functional/playlist_scheduler/Recordandcompare/newmodel8k_4_tones.wav'
        #model_address = 'SystemTests/projects/ambifier/newmodel8k_4_tones.wav'
    debug = 0
    runTests = 1
    verbose = 0
    threshold_value = 0.05              #higher = more forgiving, lower = more precise
    ###settings for recording from the sound input
    #fs = 44100
    fs = 8000          #model sample rate is 8000
    duration = 70      #of recording, in seconds. only tests that fit entirely in the duration of the recording will be run.
    #deviceId="9e8508cbd08f7bb645b8ebc815c5b9ed"    #media scheduler device
    #playlistId = "207"                             #chirps playlist
    #jwt = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwOlwvXC9leGFtcGxlLm9yZyIsImF1ZCI6Imh0dHA6XC9cL2V4YW1wbGUuY29tIiwiaWF0IjoxMzU2OTk5NTI0LCJuYmYiOjEzNTcwMDAwMDAsImxvZ2dlZEluQXMiOiI2MGY3ZGY3MDdjMTFjIn0.9mdsqyGvG-bqIQRFT66AnxeWusHXDdVu7vqpo-41Sxo" #media scheduler device
    devmanApi = devman.Devman()
    baseUrl='https://devport.inx-systems.com'

    def setDevicePlaylist(deviceId, playlistId, jwt):
        url=baseUrl+'/api/mediaDashboard.php/cms/playlistSubmitter'
        headers = {'Content-Type': 'application/json', 'Authorization': jwt}
        response = requests.post(url,json={"pairs":{deviceId:playlistId}},headers=headers,timeout=25)
        print (response)
        return response

    def recordTestAudio():
        testrecording = sd.rec(int(duration * fs), samplerate = fs, channels=1, blocking=True)
        sd.wait()
        wavfile.write(test_address, fs, testrecording)

    def normaliseArray(origArray):
        normalisation = np.linalg.norm(origArray)
        normal_array = origArray/normalisation
        print("normalisation: ", normalisation)
        return normal_array
    
    response = setDevicePlaylist(deviceId, playlistId, jwt)
    print(response.text)

    ###recording audio
    if debug == 0:
        recordTestAudio()

    ##getting sample data from sample audio
    sample_rate, samples = wavfile.read(test_address)
    
    #spectrogram data
    fftsize = 1024
    frequencyrange = int(fftsize/4)
    frequencies, times, spectrogram = signal.spectrogram(samples, sample_rate, nperseg=fftsize, noverlap =0, mode='magnitude')

    sample_duration = np.max(times)
    sample_duration_round = int(math.ceil(sample_duration / 100.0)) * 100
    #samples = array of values
    #sample_rate = sampling frequency
    #nperseg = length of each segment
    #noverlap = points to overlap between segments
    #mode = absolute magnitude of fourier transform

    #returns:
    #frequencies = array of frequencies
    #times = array of segment times
    #the spectrogram

    ##getting model data from model audio
    #if ( len(sys.argv) > 1 and sys.argv[1] == 'create_template') :
    if (1):
        #print ("Creating template spectrogram : template_variables.pik" )
        model_sample_rate, model_samples = wavfile.read(model_address)
        print("model sample rate: ", model_sample_rate)
        model_frequencies, model_times, model_spectrogram = signal.spectrogram(model_samples, model_sample_rate, nperseg=fftsize, noverlap =0, mode='magnitude')
        #filehandle = open('template_variables.pik', 'wb')
        #pickle.dump([model_frequencies, model_times, model_spectrogram],filehandle)
        #filehandle.close()
    else:
        filehandle = open('template_variables.pik', 'rb')
        model_frequencies, model_times, model_spectrogram = pickle.load(filehandle)
        filehandle.close()

    model_sum = np.sum(np.abs(model_spectrogram), 0)
    test_sum = np.sum(np.abs(spectrogram), 0)
    model_clip_value = max(model_sum) / 1.3
    test_clip_value = max(test_sum) / 1.3
    clip_spectrogram = test_sum > test_clip_value
    model_clip_spectrogram = model_sum > model_clip_value
    model_clip_indexes_greaterthan = np.where(model_clip_spectrogram == 1)
    if len(model_clip_indexes_greaterthan[0]) > 0:
        model_start_index = np.min(np.where(model_clip_spectrogram == 1))
    else: sys.exit("Could not find model white noise")

    test_clip_indexes_greaterthan = np.where(clip_spectrogram == 1)
    if len(test_clip_indexes_greaterthan[0]) > 0 :
        test_start_index = np.min(np.where(clip_spectrogram == 1))    
    else: sys.exit("Could not find test white noise")

    #print("test start index:")
    #print(test_start_index)

    if ((len(model_times)-model_start_index) >  (len(times)-test_start_index)):
        view_length=len(times) - test_start_index
    else:
        view_length=len(model_times) - model_start_index

    testSpectroValues=np.abs(normaliseArray(spectrogram))
    modelSpectroValues=np.abs(normaliseArray(model_spectrogram))

    testSpectroNorm = testSpectroValues
    modelSpectroNorm = modelSpectroValues

    norm_spectrogram_diff = testSpectroNorm[:,test_start_index:test_start_index+view_length]-modelSpectroNorm[:,model_start_index:model_start_index+view_length]
    testSpectroNorm = testSpectroNorm[:,test_start_index:test_start_index+view_length]
    modelSpectroNorm = modelSpectroNorm[:,model_start_index:model_start_index+view_length]
    #test values: sum of the difference between the spectra
    testValues=np.sum(np.abs(norm_spectrogram_diff),0)
    normTestSpectroValues = np.sum(np.abs(testSpectroNorm),0)
    normModelSpectroValues = np.sum(np.abs(modelSpectroNorm),0)

    TestLUT = [
            [0, 3, '00:59:52-00:59:55', 'White_noise', np.nan] ,
            [8, 23, '01:00:00-01:00:15', 'next (15s)(No repeats no duration) - tone 1', np.nan] ,
            [23, 38, '01:00:15-01:00:30', 'next (15s)(No repeats no duration) - tone 2', np.nan] , 
            [38, 53, '01:00:30-01:00:45', 'next (15s)(No repeats no duration) - tone 3', np.nan] , 
            [53, 68, '01:00:45-01:01:00', 'next (15s)(No repeats no duration) - tone 4', np.nan] ,

            [83, 98, '01:01:15-01:01:30', 'start time (15s)(No repeats no duration) - tone 1', np.nan] ,
            [98, 113, '01:01:30-01:01:45', 'start time (15s)(No repeats no duration) - tone 2', np.nan] ,
            [113, 128, '01:01:45-01:02:00', 'start time (15s)(No repeats no duration) - tone 3', np.nan] ,
            [128, 143, '01:02:00-01:02:15', 'start time (15s)(No repeats no duration) - tone 4', np.nan] ,

            #[158, 183, '01:02:30-01:02:55', 'start time (15s 25s)(No repeats no duration) - tone 1', np.nan] ,     #these array entries last for 25 seconds
            #[183, 208, '01:02:55-01:03:20', 'start time (15s 25s)(No repeats no duration) - tone 2', np.nan] ,     #but in the model audio the tones last for 15 seconds
            #[208, 233, '01:03:20-01:03:45', 'start time (15s 25s)(No repeats no duration) - tone 3', np.nan] ,     #correcting below
            [158, 173, '01:02:30-01:02:45', 'start time (15s 25s)(No repeats no duration) - tone 1', np.nan] ,      
            [183, 198, '01:02:55-01:03:20', 'start time (15s 25s)(No repeats no duration) - tone 2', np.nan] ,
            [208, 223, '01:03:20-01:03:45', 'start time (15s 25s)(No repeats no duration) - tone 3', np.nan] ,
            [233, 248, '01:03:45-01:04:00', 'start time (15s 25s)(No repeats no duration) - tone 4', np.nan] ,

            [263, 278, '01:04:15-01:04:30', 'start end time (15s 15s)(No repeats no duration) - tone 1', np.nan] ,
            [278, 393, '01:04:30-01:04:45', 'start end time (15s 15s)(No repeats no duration) - tone 2', np.nan] ,
            [293, 308, '01:04:45-01:05:00', 'start end time (15s 15s)(No repeats no duration) - tone 3', np.nan] ,
            [308, 323, '01:05:00-01:05:15', 'start end time (15s 15s)(No repeats no duration) - tone 4', np.nan] ,
            [338, 353, '01:05:30-01:05:45', 'end time (15m 15s)(No repeats no duration) - tone 1', np.nan] ,
            [353, 368, '01:05:45-01:06:00', 'end time (15m 15s)(No repeats no duration) - tone 2', np.nan] ,
            [368, 383, '01:06:00-01:06:15', 'end time (15m 15s)(No repeats no duration) - tone 3', np.nan] ,
            [383, 398, '01:06:15-01:06:30', 'end time (15m 15s)(No repeats no duration) - tone 4', np.nan] ,
            [413, 423, '01:06:45-01:06:55', 'next (15s)(No repeats 10s duration) - tone 1', np.nan] ,
            [423, 433, '01:06:55-01:07:05', 'next (15s)(No repeats 10s duration) - tone 2', np.nan] ,
            [433, 443, '01:07:05-01:07:15', 'next (15s)(No repeats 10s duration) - tone 3', np.nan] ,
            [443, 453, '01:07:15-01:07:25', 'next (15s)(No repeats 10s duration) - tone 4', np.nan] ,
            [468, 483, '01:07:40-01:07:55', 'start time (15s)(No repeats 10s duration) - tone 1', np.nan] ,
            [483, 498, '01:07:55-01:08:10', 'start time (15s)(No repeats 10s duration) - tone 2', np.nan] ,
            [498, 513, '01:08:10-01:08:25', 'start time (15s)(No repeats 10s duration) - tone 3', np.nan] ,
            [513, 528, '01:08:25-01:08:40', 'start time (15s)(No repeats 10s duration) - tone 4', np.nan] ,
            [543, 568, '01:08:55-01:09:20', 'start time (15s 25s)(No repeats 10s duration) - tone 1', np.nan] ,
            [568, 593, '01:09:20-01:09:45', 'start time (15s 25s)(No repeats 10s duration) - tone 2', np.nan] ,
            [593, 618, '01:09:45-01:10:10', 'start time (15s 25s)(No repeats 10s duration) - tone 3', np.nan] ,
            [618, 643, '01:10:10-01:10:35', 'start time (15s 25s)(No repeats 10s duration) - tone 4', np.nan] ,
            [643, 658, '01:10:35-01:10:50', 'next (15s)(No repeats 15s duration) - tone 1', np.nan] ,
            [658, 673, '01:10:50-01:11:05', 'next (15s)(No repeats 15s duration) - tone 2', np.nan] ,
            [673, 688, '01:11:05-01:11:20', 'next (15s)(No repeats 15s duration) - tone 3', np.nan] ,
            [688, 703, '01:11:20-01:11:35', 'next (15s)(No repeats 15s duration) - tone 4', np.nan] ,
            [718, 733, '01:11:50-01:12:05', 'start time (15s)(No repeats 15s duration) - tone 1', np.nan] ,
            [733, 748, '01:12:05-01:12:20', 'start time (15s)(No repeats 15s duration) - tone 2', np.nan] ,
            [748, 763, '01:12:20-01:12:35', 'start time (15s)(No repeats 15s duration) - tone 3', np.nan] ,
            [763, 778, '01:12:35-01:12:50', 'start time (15s)(No repeats 15s duration) - tone 4', np.nan] ,
            [793, 818, '01:13:05-01:13:30', 'start time (15s 25s)(No repeats 15s duration) - tone 1', np.nan] ,
            [818, 843, '01:13:30-01:13:55', 'start time (15s 25s)(No repeats 15s duration) - tone 2', np.nan] ,
            [843, 868, '01:13:55-01:14:20', 'start time (15s 25s)(No repeats 15s duration) - tone 3', np.nan] ,
            [868, 893, '01:14:20-01:14:45', 'start time (15s 25s)(No repeats 15s duration) - tone 4', np.nan] ,
            [908, 923, '01:15:00-01:15:15', 'next (15s)(No repeats 20s duration) - tone 1', np.nan] ,
            [923, 938, '01:15:15-01:15:30', 'next (15s)(No repeats 20s duration) - tone 2', np.nan] ,
            [938, 953, '01:15:30-01:15:45', 'next (15s)(No repeats 20s duration) - tone 3', np.nan] ,
            [953, 968, '01:15:45-01:16:00', 'next (15s)(No repeats 20s duration) - tone 4', np.nan] ,
            [983, 998, '01:16:15-01:16:30', 'start time(15s 15s)(No repeats 20s duration) - tone 1', np.nan] ,
            [998, 1013, '01:16:30-01:16:45', 'start time(15s 15s)(No repeats 20s duration) - tone 2', np.nan] ,
            [1013, 1028, '01:16:45-01:17:00', 'start time(15s 15s)(No repeats 20s duration) - tone 3', np.nan] ,
            [1028, 1043, '01:17:00-01:17:15', 'start time(15s 15s)(No repeats 20s duration) - tone 4', np.nan] ,
            [1058, 1083, '01:17:30-01:17:55', 'start time(15s 25s)(No repeats 20s duration) - tone 1', np.nan] ,
            [1083, 1108, '01:17:55-01:18:20', 'start time(15s 25s)(No repeats 20s duration) - tone 2', np.nan] ,
            [1108, 1133, '01:18:20-01:18:45', 'start time(15s 25s)(No repeats 20s duration) - tone 3', np.nan] ,
            [1133, 1158, '01:18:45-01:19:10', 'start time(15s 25s)(No repeats 20s duration) - tone 4', np.nan] ,
            [1173, 1218, '01:19:25-01:20:10', 'next(15s)(3 repeats no duration) - tone 1', np.nan] ,
            [1218, 1263, '01:20:10-01:20:55', 'next(15s)(3 repeats no duration) - tone 2', np.nan] ,
            [1263, 1308, '01:20:55-01:21:40', 'next(15s)(3 repeats no duration) - tone 3', np.nan] ,
            [1308, 1353, '01:21:40-01:22:25', 'next(15s)(3 repeats no duration) - tone 4', np.nan] ,
            [1368, 1413, '01:22:40-01:23:25', 'start time(15s)(3 repeats no duration) - tone 1', np.nan] ,
            [1413, 1458, '01:23:25-01:24:10', 'start time(15s)(3 repeats no duration) - tone 2', np.nan] ,
            [1458, 1503, '01:24:10-01:24:55', 'start time(15s)(3 repeats no duration) - tone 3', np.nan] ,
            [1503, 1548, '01:24:55-01:25:40', 'start time(15s)(3 repeats no duration) - tone 4', np.nan] ,
            [1563, 1623, '01:25:55-01:26:55', 'start end time(15s)(3 repeats no duration) - tone 1', np.nan] ,
            [1623, 1683, '01:26:55-01:27:55', 'start end time(15s)(3 repeats no duration) - tone 2', np.nan] ,
            [1683, 1743, '01:27:55-01:28:55', 'start end time(15s)(3 repeats no duration) - tone 3', np.nan] ,
            [1743, 1803, '01:28:55-01:29:55', 'start end time(15s)(3 repeats no duration) - tone 4', np.nan] ,
            [1818, 1863, '01:30:10-01:30:55', 'end time(15s)(3 repeats no duration) - tone 1', np.nan] ,
            [1863, 1908, '01:30:55-01:31:40', 'end time(15s)(3 repeats no duration) - tone 2', np.nan] ,
            [1908, 1953, '01:31:40-01:32:25', 'end time(15s)(3 repeats no duration) - tone 3', np.nan] ,
            [1953, 1998, '01:32:25-01:33:10', 'end time(15s)(3 repeats no duration) - tone 4', np.nan] ,
            [2013, 2043, '01:33:25-01:33:55', 'next (15s)(3 repeats 10s duration) - tone 1', np.nan] ,
            [2043, 2073, '01:33:55-01:34:25', 'next (15s)(3 repeats 10s duration) - tone 2', np.nan] ,
            [2073, 2103, '01:34:25-01:34:55', 'next (15s)(3 repeats 10s duration) - tone 3', np.nan] ,
            [2103, 2133, '01:34:55-01:35:25', 'next (15s)(3 repeats 10s duration) - tone 4', np.nan] ,
            [2148, 2178, '01:35:40-01:36:10', 'start time (15s 15s)(3 repeats 10s duration) - tone 1', np.nan] ,
            [2178, 2208, '01:36:10-01:36:40', 'start time (15s 15s)(3 repeats 10s duration) - tone 2', np.nan] ,
            [2208, 2238, '01:36:40-01:37:10', 'start time (15s 15s)(3 repeats 10s duration) - tone 3', np.nan] ,
            [2238, 2268, '01:37:10-01:37:40', 'start time (15s 15s)(3 repeats 10s duration) - tone 4', np.nan] ,
            [2283, 2328, '01:37:55-01:38:40', 'next (15s)(3 repeats 15s duration) - tone 1', np.nan] ,
            [2328, 2373, '01:38:40-01:39:25', 'next (15s)(3 repeats 15s duration) - tone 2', np.nan] ,
            [2373, 2418, '01:39:25-01:40:10', 'next (15s)(3 repeats 15s duration) - tone 3', np.nan] ,
            [2418, 2463, '01:40:10-01:40:55', 'next (15s)(3 repeats 15s duration) - tone 4', np.nan] ,
            [2463, 2508, '01:40:55-01:41:40', 'start time(15s 15s)(3 repeats 15s duration) - tone 1', np.nan] ,
            [2508, 2553, '01:41:40-01:42:25', 'start time(15s 15s)(3 repeats 15s duration) - tone 2', np.nan] ,
            [2553, 2598, '01:42:25-01:43:10', 'start time(15s 15s)(3 repeats 15s duration) - tone 3', np.nan] ,
            [2598, 2643, '01:43:10-01:43:55', 'start time(15s 15s)(3 repeats 15s duration) - tone 4', np.nan] ,
            [2658, 2718, '01:44:10-01:45:10', 'next (15s)(3 repeats 20s duration) - tone 1', np.nan] ,
            [2718, 2778, '01:45:10-01:46:10', 'next (15s)(3 repeats 20s duration) - tone 2', np.nan] ,
            [2778, 2838, '01:46:10-01:47:10', 'next (15s)(3 repeats 20s duration) - tone 3', np.nan] ,
            [2838, 2898, '01:47:10-01:48:10', 'next (15s)(3 repeats 20s duration) - tone 4', np.nan] ,
            [2913, 2973, '01:48:25-01:49:25', 'start time (15s 15s)(3 repeats 20s duration) - tone 1', np.nan] ,
            [2973, 3033, '01:49:25-01:50:25', 'start time (15s 15s)(3 repeats 20s duration) - tone 2', np.nan] ,
            [3033, 3093, '01:50:25-01:51:25', 'start time (15s 15s)(3 repeats 20s duration) - tone 3', np.nan] ,
            [3093, 3153, '01:51:25-01:52:25', 'start time (15s 15s)(3 repeats 20s duration) - tone 4', np.nan]
            ]
        
    npTestLUT = np.array(TestLUT)

    #list of test values with test number
    test_results_list = {}

    if (runTests == 1):
        test_cases_list =[]
        for index, tone in enumerate(TestLUT):  #tone = sub-array
            timesTesting = np.where(times >= tone[0] )  #array of times > this array's first value
            testCase1 = tests.TestCase()
            testCase1.classname='inx.ambifier.tests.media_scheduler'
            testCase1.name=deviceId
            testCase1.time=str(time.time())
            if (len(timesTesting[0]) > 0 ) :
                start_index = np.min(timesTesting) #start where the lowest start-time is
                end_index = np.max(np.where(times <= tone[1])) #end where the end-time is > times
                average_test_value = np.sum(testValues[start_index:end_index])/(end_index-start_index) #[sum starting at the start index and stopping before the end index]/number of indices
                print('avg test val =', average_test_value)
                tone[4] = average_test_value 
                if (average_test_value > threshold_value):
                    stat= "fail"
                    #print (tone[3], stat)
                    npTestLUT[index][4] = average_test_value
                    test_results_list[index] = {}
                    test_results_list[index]['test_value'] = average_test_value
                    test_results_list[index]['status'] = stat
                    test_results_list[index]['number'] = index
                    test_results_list[index]['times'] = npTestLUT[index][2]
                    testCase1.systemout=str(average_test_value)
                    testCase1.systemerr="average test value above threshold"
                else:
                    stat= "pass"
                    #print (tone[3], stat)
                    npTestLUT[index][4] = average_test_value
                    test_results_list[index] = {}
                    test_results_list[index]['test_value'] = average_test_value
                    test_results_list[index]['status'] = stat
                    test_results_list[index]['number'] = index
                    test_results_list[index]['times'] = npTestLUT[index][2]
                    testCase1.systemout=str(average_test_value)
            else:
                #print("skipping tone, not within specified times")
                stat= 'skipped'
                #print(tone[3], stat)
                npTestLUT[index][4] = average_test_value
                testCase1.systemout="test skipped, not within specified times"
                #test_results_list[index] = {}
                #test_results_list[index]['status'] = stat
        #    index =+ 1
            #, status=stat
            testSuite1.addTestCase(testCase1)
            test_entry = ju.TestCase(name = tone[3])
            if stat == "fail":
                test_entry.add_failure_info(message=stat)
            elif stat == "skipped":
                test_entry.add_skipped_info(message=stat)
            
            test_cases_list.append(test_entry)
        #ts = ju.TestSuite("schedulerstuff",test_cases_list)
        #print(ju.TestSuite.to_xml_string([ts]))
        #with open('output.xml', 'w') as f:
            #ju.TestSuite.to_file(f, [ts], prettyprint=False)
        #print("list of results: ", test_results_list)
        for i in test_results_list:
            print(test_results_list[i])

    #write the results out
    tests.writeJunitFile('mediaScheduler',testSuites)

    #displaying graphs
    if (verbose == 1):
        f, ax = plt.subplots(5, sharex = True, sharey = False)#, sharey = True
        ax[0].pcolormesh(times[0:view_length], frequencies[1:frequencyrange], spectrogram[1:frequencyrange,test_start_index:test_start_index+view_length], cmap='binary')
        ax[1].pcolormesh(times[0:view_length], model_frequencies[1:frequencyrange], model_spectrogram[1:frequencyrange,model_start_index:model_start_index+view_length], cmap='binary')
        #model spectro with test times
        #ax[2].pcolormesh(times[0:view_length], model_frequencies[1:frequencyrange], model_spectrogram[1:frequencyrange,test_start_index:test_start_index+view_length], cmap='binary')
        ax[0].set_ylim(0,1200)
        #ax[0].set_xlim(auto=True)
        ax[1].set_ylim(0,1200)
        plt.xlabel('Time [sec]')
        ax[0].title.set_text('Test audio')
        ax[1].title.set_text('Model audio')
        ax[0].set_ylabel('Frequency/Hz')
        ax[1].set_ylabel('Frequency/Hz')
        #print('testlutslice =', npTestLUT[1:20, 0])#npTestLUT[:,4])
    #    plt.Figure.add_subplot(551)

        ax[2].plot(times[0:view_length],testValues) #blue line - difference between spectra over time
        ax[2].plot(npTestLUT[:, 0].astype(int), 0.1* np.ones(np.shape(npTestLUT[:, 4])),'r*')   #red stars - where the test starting points are
        ax[2].set_ylim(0,0.2)  
        ax[2].set_xlim(0, sample_duration_round)
        ax[2].title.set_text('Test/model difference, normalised')
        ax[2].set_ylabel('Amount of freq')
    #    plt.plot(npTestLUT[:,1],npTestLUT[:,4],'r+')
    #    plt.plot(npTestLUT[:,0],npTestLUT[:,4],'g+')

        ax[3].plot(times[0:view_length],normTestSpectroValues) #blue line - difference between spectra over time
        #ax[3].plot(npTestLUT[:, 0].astype(int), 0.1* np.ones(np.shape(npTestLUT[:, 4])),'r*')   #red stars - where the test starting points are
        ax[3].set_ylim(0,0.25)  
        ax[3].set_xlim(0, sample_duration_round)
        ax[3].title.set_text('Test audio values, normalised')
        ax[3].set_ylabel('Amount of freq')

        ax[4].plot(times[0:view_length],normModelSpectroValues) #blue line - difference between spectra over time
        #ax[4].plot(npTestLUT[:, 0].astype(int), 0.1* np.ones(np.shape(npTestLUT[:, 4])),'r*')   #red stars - where the test starting points are
        ax[4].set_ylim(0,0.25)  
        ax[4].set_xlim(0, sample_duration_round)
        ax[4].title.set_text('Model audio values, normalised')
        ax[4].set_ylabel('Amount of freq')
        plt.show()

