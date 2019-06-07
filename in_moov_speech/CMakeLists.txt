cmake_minimum_required(VERSION 2.8.3)
project(in_moov_speech)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
  tf
  roscpp 
)
catkin_python_setup()
in_moov_speech/
    CMakesLists.txt
    package.xml
    launch/ # roslaunch files go here
    msg/ # msg files go here
    nodes/ # installable python nodes go here
        txt2speech.py
        speech2txt.py
    src/ # this is where your python modules exported as libraries or used in your nodes go.
    urdf/ # urdf and xacro files go here -- basically your robot model stuff, if any.
    scripts/ # generally non-exported python scripts -- stuff that might be called to setup your package and code gen stuff
    srv/ # service descriptions go here
    __init__.py # only necessary if you want to use anything from the scripts directory in your package src and node files
    setup.py # more on this later
