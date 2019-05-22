#Based on the example file from the marytts GitHub
#modified by Waldemar Drozd 
# -*- coding: utf-8 -*-

# HTTP + URL packages
import httplib2
try:
    from urllib.parse import urlencode, quote # For URL creation
except ImportError:                     #make code compatible to Python 2 & 3 
    from urlib import urlencode, quote

     
# To play wave files
import pygame
import math # For ceiling


# Mary server informations
mary_host = "localhost"
mary_port = "59125"

# Input text
f = open("test.txt", "r") #open the string from .txt
if f.mode == 'r':         #checks if the file is readable
    textString = f.read()
input_text = textString

# Build the query
query_hash = {"INPUT_TEXT":input_text,
              "INPUT_TYPE":"TEXT", # Input text
              "LOCALE":"en_US",
              "VOICE":"cmu-bdl-hsmm", # inMoov male voice
              "OUTPUT_TYPE":"AUDIO",
              "AUDIO":"WAVE", # Audio informations (need both)
              }
query = urlencode(query_hash)
print("query = \"http://%s:%s/process?%s\"" % (mary_host, mary_port, query))

# Run the query to mary http server
h_mary = httplib2.Http()
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
