#!/usr/bin/env python

#Based on the example file from the marytts GitHub
#modified by Waldemar Drozd 
# -*- coding: utf-8 -*-

#ros libs
import rospy
from std_msgs.msg import String

# HTTP + URL packages
import httplib2
try:
    from urllib.parse import urlencode, quote # For URL creation
except ImportError:                     #make code compatible to Python 2 & 3 
    from urllib import urlencode, quote

import pygame # To play wave files
import math # For ceiling

def callback(input_text):
     rospy.loginfo(rospy.get_caller_id())
    
     mary_host = "localhost" # Mary server information
     mary_port = "59125"
    
     # Build the query
     query_hash = {"INPUT_TEXT":input_text,
    "INPUT_TYPE":"TEXT", # Input text
    "LOCALE":"en_US", #language
    "VOICE":"cmu-bdl-hsmm", # inMoov male voice
    "OUTPUT_TYPE":"AUDIO",
    "AUDIO":"WAVE", # Audio informations (need both)
    }
     query = urlencode(query_hash)
     print("query = \"http://%s:%s/process?%s\"" % (mary_host, mary_port, query))
    
     h_mary = httplib2.Http() # Run the query to mary http server
     resp, content = h_mary.request("http://%s:%s/process?" % (mary_host, mary_port), "POST", query)
    
     #  Decode the wav file or raise an exception if no wav files
     if (resp["content-type"] == "audio/x-wav"):
       
      # Write the wav file
      f = open("/tmp/output_wav.wav", "wb")
      f.write(content)
      f.close()
    
      # Play the wav file
      pygame.mixer.init(frequency=16000) # Initialise the mixer
      s = pygame.mixer.Sound("/tmp/output_wav.wav")
      s.play()
      pygame.time.wait(int(math.ceil(s.get_length() * 10)))
    
     else:
        raise Exception(content)

def txt2speech():
    rospy.init_node('txt2speech', anonymous=True) #name of subscriber node
    rospy.Subscriber("speaker", String, callback) #name of topic
    rospy.spin() #keeps python from exiting until node is stopped
    
if __name__ == '__main__':
    txt2speech()