#!/usr/bin/env python

# -*- coding: utf-8 -*-
"""

@author: Waldemar Drozd
"""

#import libs
import rospy
from std_msgs.msg import String
import speech_recognition as sr

def speech2txt():
    pub = rospy.Publisher('recognizer', String, queue_size=10) #name of topic
    rospy.init_node('speech2txt', anonymous=True) #name of publisher node
    rate = rospy.Rate(10) # 10hz
    r = sr.Recognizer()
    mic = sr.Microphone()
    mic.CHUNK = 1024  #adjust chunk size
    
    while not rospy.is_shutdown():
        with mic as source:
            r.adjust_for_ambient_noise(source)  #adjust mic to ambient noice
            audio = r.listen(source)
            try:
                text = r.recognize_google(audio)
                rospy.loginfo(text)
                pub.publish(text)
            except sr.UnknownValueError:
                pub.publish('Unknown Value Error')
            except sr.RequestError as Er:
                pub.publish('Cannot obtain results')
            except:
                pub.publish('Sorry, I could not understand you')
                
if __name__ == '__main__':
    try:
        speech2txt()
    except rospy.ROSInterruptException:
        pass
