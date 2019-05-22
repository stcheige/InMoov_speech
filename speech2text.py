
# -*- coding: utf-8 -*-
"""
Created on Wed May 22 15:53:39 2019

@author: Waldemar Drozd
"""

import speech_recognition as sr
r = sr.Recognizer()
mic = sr.Microphone()
mic.CHUNK = 1024
with mic as source:
    print('Please speak : ')
    r.adjust_for_ambient_noise(source)
    audio = r.listen(source)

try:
    text = r.recognize_google(audio)
    file = open('test.txt', 'w' )
    file.write(text)
    file.close()
    print('You said : {}'.format(text))
    
except sr.UnknownValueError:
    print('Audio not')
except sr.RequestError as Er:
    print('Cannot obtain results; {0}'.format(Er))
except:
    print('Sorry, I could not understand you')
