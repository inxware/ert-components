# -*- coding: utf-8 -*-
"""
Created on Tue Dec 11 09:27:53 2018

@author: inx limited
"""
from scipy import signal
import matplotlib.pyplot as plt
from scipy.io import wavfile

audio = wavfile.read('audio.wav')
f, t, Sxx = signal.spectrogram(audio)
plt.pcolormesh(t, f, Sxx)
plt.ylabel('Frequency [Hz]')
plt.xlabel('Time [sec]')
plt.show()