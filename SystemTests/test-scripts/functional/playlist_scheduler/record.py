# -*- coding: utf-8 -*-
"""
Created on Wed Dec 19 15:32:52 2018

@author: inx limited
"""

#import numpy as np
import sounddevice as sd
import scipy.io.wavfile

fs = 44100

duration = 300

testrecording = sd.rec(int(duration * fs), samplerate = fs, channels=1, blocking=True)
#print(np.shape(testrecording))
scipy.io.wavfile.write('/root/opt/test_models/newestmodel.wav', fs, testrecording)