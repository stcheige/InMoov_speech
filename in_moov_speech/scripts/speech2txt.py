#!/usr/bin/env python


# -*- coding: utf-8 -*-
"""

@author: Waldemar Drozd
"""

#import libs
import rospy #lib for ROS 
from std_msgs.msg import String #sending Messages as string
import speech_recognition as sr #lib for speech recognition


def speech2txt():
    pub = rospy.Publisher('chatIN', String, queue_size=10) #name of topic
    rospy.init_node('speech2txt', anonymous=True) #name of publisher node
    rate = rospy.Rate(10) # 10hz
    rec = sr.Recognizer() 
    mic = sr.Microphone()
    mic.CHUNK = 1024  #adjust chunk size
    
    while not rospy.is_shutdown():
        print("Listening") #print into the terminal
        with mic as source: #using the microphone to record audio
            rec.adjust_for_ambient_noise(source)  #adjust mic to ambient noice
            audio = rec.listen(source) #record audio 
            try:
                text = rec.recognize_google(audio) #send audio and receive transcribed audio as text
		print("Recognizing")
                rospy.loginfo(text)
                pub.publish(text) #publish received text 
                
            except sr.UnknownValueError:
                pub.publish('Sorry I could not understand you')
                print("Sorry I could not understand you")
            except sr.RequestError:
                pub.publish('Can not obtain results')
                print("Can not obtain results")
           
                
if __name__ == '__main__':
    try:
        speech2txt()
    except rospy.ROSInterruptException:
        pass
