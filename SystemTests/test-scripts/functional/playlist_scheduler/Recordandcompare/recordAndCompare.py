# -*- coding: utf-8 -*-
"""
Created on Thu Dec 20 11:44:34 2018

@author: inx limited
"""

##Necessary modules
import sounddevice as sd
#import matplotlib.pyplot as plt
from scipy import signal
from scipy.io import wavfile
import numpy
import sys
import pickle
import junit_xml as ju
import requests

fs = 44100
duration = 33

model_files="/opt/test_models/"

deviceID="94:C6:91:1E:B9:18"
playlistPath = "Testing.xml"
requests.post("http://192.168.2.202:8000/index.php",json='{"deviceId":"'+deviceID+'","playlist":"_data/'+playlistPath+'"}')

#--data '{"deviceId":"'$1'","playlist":"_data/'$2'"}' http://server/api/v1/content/sendplaylist.php
testrecording = sd.rec(int(duration * fs), samplerate = fs, channels=1, blocking=True)
wavfile.write('/root/opt/test_models/Test.wav', fs, testrecording)

sample_rate, samples = wavfile.read('/root/opt/test_models/Test.wav')


fftsize = 1024
frequencyrange = int(fftsize/4)
frequencies, times, spectrogram = signal.spectrogram(samples, sample_rate, nperseg=fftsize, noverlap =0, mode='magnitude')




if ( len(sys.argv) > 1 and sys.argv[1] == 'create_template') :
    print ("Creating template spectrogram : template_variables.pik" )
    model_sample_rate, model_samples = wavfile.read('/root/opt/test_models/newestmodel.wav') 
    model_frequencies, model_times, model_spectrogram = signal.spectrogram(model_samples, model_sample_rate, nperseg=fftsize, noverlap =0, mode='magnitude')
    filehandle = open('/root/opt/test_models/template_variables.pik', 'wb')
    pickle.dump([model_frequencies, model_times, model_spectrogram],filehandle)
    filehandle.close()
else:
    filehandle = open('/root/opt/test_models/template_variables.pik', 'rb')
    model_frequencies, model_times, model_spectrogram = pickle.load(filehandle)
    filehandle.close()


model_sum = numpy.sum(numpy.abs(model_spectrogram), 0)
test_sum = numpy.sum(numpy.abs(spectrogram), 0)
model_clip_value = max(model_sum) / 1.3
test_clip_value = max(test_sum) / 1.3
clip_spectrogram = test_sum > model_clip_value
model_clip_spectrogram = model_sum > model_clip_value
print(model_clip_value)
print(test_clip_value)
if sum(model_clip_spectrogram) == 0:
    sys.exit("Could not find model white noise")
if sum(clip_spectrogram) == 0:
    sys.exit("Could not find test white noise")

#model_clip_indexes_greaterthan = numpy.where(model_clip_spectrogram == 1)
#if len(model_clip_indexes_greaterthan[0]) > 0:
model_start_index = min(numpy.where(model_clip_spectrogram == 1))

#test_clip_indexes_greaterthan = numpy.where(clip_spectrogram == 1)
#if len(test_clip_indexes_greaterthan[0]) > 0 :
test_start_index = min(numpy.where(clip_spectrogram == 1))

if ((len(model_times)-min(model_start_index)) >  (len(times)-min(test_start_index))):
    view_length=len(times) - test_start_index
else:
    view_length=len(model_times) - model_start_index


test_start_index = min(test_start_index)
model_start_index = min(model_start_index)
spectrogram_diff = spectrogram[:,test_start_index:test_start_index+len(view_length)]-model_spectrogram[:,model_start_index:model_start_index+len(view_length)]
if ( 1 ) :
    #f, ax = plt.subplots(3, sharex = True, sharey = False)#, sharey = True #
    #ax[0].pcolormesh(times[0:view_length], frequencies[1:frequencyrange], spectrogram[1:frequencyrange,test_start_index:test_start_index+view_length], cmap='binary')
    #ax[1].pcolormesh(times[0:view_length], model_frequencies[1:frequencyrange], model_spectrogram[1:frequencyrange,model_start_index:model_start_index+view_length], cmap='binary')
    #ax[0].set_ylim(0,1200)
    #ax[0].set_xlim(auto=True)
    #ax[1].set_ylim(0,1200)
    #plt.xlabel('Time [sec]')
    testValues=numpy.sum(numpy.abs(spectrogram_diff),0)
    #ax[2].plot(times[0:view_length],testValues)
    #ax[2].set_ylim(0,1.2)
    #plt.show()
    
    
    
    
TestLUT=[
     [0, 1,"00:59:52-00:59:55","White_noise",-1],
     [6, 21,"01:00:00-01:00:15","next (15s)(No repeats no duration) - tone 1",-1],
     [22, 37,"01:00:15-01:00:30","next (15s)(No repeats no duration) - tone 2",-1],
     [38, 53,"01:00:30-01:00:45","next (15s)(No repeats no duration) - tone 3",-1],
     [54, 69,"01:00:45-01:01:00","next (15s)(No repeats no duration) - tone 4",-1],
     [85, 100,"01:01:15-01:01:30","start time (15s)(No repeats no duration) - tone 1",-1],
     [101, 116,"01:01:30-01:01:45","start time (15s)(No repeats no duration) - tone 2",-1],
     [117, 132,"01:01:45-01:02:00","start time (15s)(No repeats no duration) - tone 3",-1],
     [133, 148,"01:02:00-01:02:15","start time (15s)(No repeats no duration) - tone 4",-1],
     [155, 180,"01:02:30-01:02:55","start time (15s 25s)(No repeats no duration) - tone 1",-1],
     [181, 206,"01:02:55-01:03:20","start time (15s 25s)(No repeats no duration) - tone 2",-1],
     [207, 232,"01:03:20-01:03:45","start time (15s 25s)(No repeats no duration) - tone 3",-1],
     [233, 258,"01:03:45-01:04:00","start time (15s 25s)(No repeats no duration) - tone 4",-1],
     [274, 289,"01:04:15-01:04:30","start end time (15s 15s)(No repeats no duration) - tone 1",-1],
     [290, 305,"01:04:30-01:04:45","start end time (15s 15s)(No repeats no duration) - tone 2",-1],
     [306, 321,"01:04:45-01:05:00","start end time (15s 15s)(No repeats no duration) - tone 3",-1],
     [321, 336,"01:05:00-01:05:15","start end time (15s 15s)(No repeats no duration) - tone 4",-1],
     [351, 366,"01:05:30-01:05:45","end time (15m 15s)(No repeats no duration) - tone 1",-1],
     [367, 382,"01:05:45-01:06:00","end time (15m 15s)(No repeats no duration) - tone 2",-1],
     [383, 398,"01:06:00-01:06:15","end time (15m 15s)(No repeats no duration) - tone 3",-1],
     [399, 414,"01:06:15-01:06:30","end time (15m 15s)(No repeats no duration) - tone 4",-1],
     [430, 440,"01:06:45-01:06:55","next (15s)(No repeats 10s duration) - tone 1",-1],
     [441, 451,"01:06:55-01:07:05","next (15s)(No repeats 10s duration) - tone 2",-1],
     [452, 462,"01:07:05-01:07:15","next (15s)(No repeats 10s duration) - tone 3",-1],
     [463, 473,"01:07:15-01:07:25","next (15s)(No repeats 10s duration) - tone 4",-1],
     [488, 503,"01:07:40-01:07:55","start time (15s)(No repeats 10s duration) - tone 1",-1],     
     [504, 519,"01:07:55-01:08:10","start time (15s)(No repeats 10s duration) - tone 2",-1],
     [520, 535,"01:08:10-01:08:25","start time (15s)(No repeats 10s duration) - tone 3",-1],
     [536, 551,"01:08:25-01:08:40","start time (15s)(No repeats 10s duration) - tone 4",-1],
     [567, 592,"01:08:55-01:09:20","start time (15s 25s)(No repeats 10s duration) - tone 1",-1],
     [593, 618,"01:09:20-01:09:45","start time (15s 25s)(No repeats 10s duration) - tone 2",-1],
     [619, 644,"01:09:45-01:10:10","start time (15s 25s)(No repeats 10s duration) - tone 3",-1],
     [645, 670,"01:10:10-01:10:35","start time (15s 25s)(No repeats 10s duration) - tone 4",-1],
     [671, 686,"01:10:35-01:10:50","next (15s)(No repeats 15s duration) - tone 1",-1],
     [687, 702,"01:10:50-01:11:05","next (15s)(No repeats 15s duration) - tone 2",-1],
     [703, 718,"01:11:05-01:11:20","next (15s)(No repeats 15s duration) - tone 3",-1],
     [719, 734,"01:11:20-01:11:35","next (15s)(No repeats 15s duration) - tone 4",-1],
     [750, 765,"01:11:50-01:12:05","start time (15s)(No repeats 15s duration) - tone 1",-1],
     [766, 781,"01:12:05-01:12:20","start time (15s)(No repeats 15s duration) - tone 2",-1],
     [782, 797,"01:12:20-01:12:35","start time (15s)(No repeats 15s duration) - tone 3",-1],
     [798, 813,"01:12:35-01:12:50","start time (15s)(No repeats 15s duration) - tone 4",-1],
     [829, 854,"01:13:05-01:13:30","start time (15s 25s)(No repeats 15s duration) - tone 1",-1],
     [855, 880,"01:13:30-01:13:55","start time (15s 25s)(No repeats 15s duration) - tone 2",-1],
     [881, 906,"01:13:55-01:14:20","start time (15s 25s)(No repeats 15s duration) - tone 3",-1],
     [907, 932,"01:14:20-01:14:45","start time (15s 25s)(No repeats 15s duration) - tone 4",-1],
     [948, 963,"01:15:00-01:15:15","next (15s)(No repeats 20s duration) - tone 1",-1],
     [964, 979,"01:15:15-01:15:30","next (15s)(No repeats 20s duration) - tone 2",-1],
     [980, 995,"01:15:30-01:15:45","next (15s)(No repeats 20s duration) - tone 3",-1],
     [996, 1011,"01:15:45-01:16:00","next (15s)(No repeats 20s duration) - tone 4",-1],
     [1027, 1042,"01:16:15-01:16:30","start time(15s 15s)(No repeats 20s duration) - tone 1",-1],
     [1043, 1058,"01:16:30-01:16:45","start time(15s 15s)(No repeats 20s duration) - tone 2",-1],
     [1059, 1074,"01:16:45-01:17:00","start time(15s 15s)(No repeats 20s duration) - tone 3",-1],
     [1075, 1090,"01:17:00-01:17:15","start time(15s 15s)(No repeats 20s duration) - tone 4",-1],
     [1106, 1031,"01:17:30-01:17:55","start time(15s 25s)(No repeats 20s duration) - tone 1",-1],
     [1132, 1157,"01:17:55-01:18:20","start time(15s 25s)(No repeats 20s duration) - tone 2",-1],
     [1158, 1183,"01:18:20-01:18:45","start time(15s 25s)(No repeats 20s duration) - tone 3",-1],
     [1184, 1209,"01:18:45-01:19:10","start time(15s 25s)(No repeats 20s duration) - tone 4",-1],
     [1225, 1270,"01:19:25-01:20:10","next(15s)(3 repeats no duration) - tone 1",-1],
     [1271, 1316,"01:20:10-01:20:55","next(15s)(3 repeats no duration) - tone 2",-1],
     [1317, 1362,"01:20:55-01:21:40","next(15s)(3 repeats no duration) - tone 3",-1],
     [1363, 1408,"01:21:40-01:22:25","next(15s)(3 repeats no duration) - tone 4",-1],
     [1424, 1469,"01:22:40-01:23:25","start time(15s)(3 repeats no duration) - tone 1",-1],
     [1470, 1515,"01:23:25-01:24:10","start time(15s)(3 repeats no duration) - tone 2",-1],
     [1516, 1561,"01:24:10-01:24:55","start time(15s)(3 repeats no duration) - tone 3",-1],
     [1562, 1607,"01:24:55-01:25:40","start time(15s)(3 repeats no duration) - tone 4",-1],
     [1623, 1683,"01:25:55-01:26:55","start end time(15s)(3 repeats no duration) - tone 1",-1],
     [1684, 1744,"01:26:55-01:27:55","start end time(15s)(3 repeats no duration) - tone 2",-1],
     [1745, 1805,"01:27:55-01:28:55","start end time(15s)(3 repeats no duration) - tone 3",-1],
     [1806, 1866,"01:28:55-01:29:55","start end time(15s)(3 repeats no duration) - tone 4",-1],
     [1882, 1927,"01:30:10-01:30:55","end time(15s)(3 repeats no duration) - tone 1",-1],
     [1928, 1973,"01:30:55-01:31:40","end time(15s)(3 repeats no duration) - tone 2",-1],
     [1974, 2019,"01:31:40-01:32:25","end time(15s)(3 repeats no duration) - tone 3",-1],
     [2020, 2065,"01:32:25-01:33:10","end time(15s)(3 repeats no duration) - tone 4",-1],
     [2081, 2111,"01:33:25-01:33:55","next (15s)(3 repeats 10s duration) - tone 1",-1],
     [2112, 2142,"01:33:55-01:34:25","next (15s)(3 repeats 10s duration) - tone 2",-1],
     [2143, 2173,"01:34:25-01:34:55","next (15s)(3 repeats 10s duration) - tone 3",-1],
     [2174, 2204,"01:34:55-01:35:25","next (15s)(3 repeats 10s duration) - tone 4",-1],
     [2219, 2249,"01:35:40-01:36:10","start time (15s 15s)(3 repeats 10s duration) - tone 1",-1],
     [2250, 2280,"01:36:10-01:36:40","start time (15s 15s)(3 repeats 10s duration) - tone 2",-1],
     [2281, 2311,"01:36:40-01:37:10","start time (15s 15s)(3 repeats 10s duration) - tone 3",-1],
     [2312, 2342,"01:37:10-01:37:40","start time (15s 15s)(3 repeats 10s duration) - tone 4",-1],
     [2358, 2403,"01:37:55-01:38:40","next (15s)(3 repeats 15s duration) - tone 1",-1],
     [2404, 2449,"01:38:40-01:39:25","next (15s)(3 repeats 15s duration) - tone 2",-1],
     [2450, 2495,"01:39:25-01:40:10","next (15s)(3 repeats 15s duration) - tone 3",-1],
     [2496, 2541,"01:40:10-01:40:55","next (15s)(3 repeats 15s duration) - tone 4",-1],
     [2542, 2587,"01:40:55-01:41:40","start time(15s 15s)(3 repeats 15s duration) - tone 1",-1],
     [2588, 2633,"01:41:40-01:42:25","start time(15s 15s)(3 repeats 15s duration) - tone 2",-1],
     [2634, 2679,"01:42:25-01:43:10","start time(15s 15s)(3 repeats 15s duration) - tone 3",-1],
     [2680, 2725,"01:43:10-01:43:55","start time(15s 15s)(3 repeats 15s duration) - tone 4",-1],
     [2741, 2801,"01:44:10-01:45:10","next (15s)(3 repeats 20s duration) - tone 1",-1],
     [2802, 2862,"01:45:10-01:46:10","next (15s)(3 repeats 20s duration) - tone 2",-1],
     [2863, 2923,"01:46:10-01:47:10","next (15s)(3 repeats 20s duration) - tone 3",-1],
     [2924, 2984,"01:47:10-01:48:10","next (15s)(3 repeats 20s duration) - tone 4",-1],
     [3000, 3060,"01:48:25-01:49:25","start time (15s 15s)(3 repeats 20s duration) - tone 1",-1],
     [3061, 3121,"01:49:25-01:50:25","start time (15s 15s)(3 repeats 20s duration) - tone 2",-1],
     [3122, 3182,"01:50:25-01:51:25","start time (15s 15s)(3 repeats 20s duration) - tone 3",-1],
     [3183, 3243,"01:51:25-01:52:25","start time (15s 15s)(3 repeats 20s duration) - tone 4",-1],


    ]

npTestLUT = numpy.array(TestLUT)
index = 0  

test_cases_list =[]
for tone in TestLUT:
    xxxx = numpy.where(times >= tone[0] )
    #print(xxxx)
    if (len(xxxx[0]) > 0 ) :
        start_index = numpy.min(xxxx)
        end_index = numpy.max(numpy.where(times <= tone[1] ))
        average_test_value = numpy.sum(testValues[start_index:end_index])/(end_index-start_index)
 
        npTestLUT[index,4] = average_test_value
        #print(npTestLUT[index,4])
        if (average_test_value > 0.25):
            print (tone[3])
            stat= "fail"
        else:
            print (tone[3])
            stat= "pass"
        index =+ 1
    else:
        print(tone[3])
        stat= 'skipped'
    test_entry = ju.TestCase(name = tone[3], status=stat)    
    if stat == "fail":
        test_entry.add_failure_info(message=stat)
    elif stat == "skipped":
        test_entry.add_skipped_info(message=stat)
    
    test_cases_list.append(test_entry)

ts = ju.TestSuite("schedulerstuff",test_cases_list)
print(ju.TestSuite.to_xml_string([ts]))
with open('output.xml', 'w') as f:
    ju.TestSuite.to_file(f, [ts], prettyprint=False)
