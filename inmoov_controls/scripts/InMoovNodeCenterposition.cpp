#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/String.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
sensor_msgs::JointState jState;

void Stateinput(string file){
    ifstream in(file.c_str());
    string s;
    float f;
    while(in.eof() ==false){
        in>> s;
        in>> f;
        jState.name.push_back (s);
        jState.position.push_back (f);
        ROS_INFO("%s %f", s.c_str(), f);
    }
    in.close();
}


int main(int argc, char **argv)
{
//Initialization for the node
  ros::init(argc, argv, "inmoov_centerposition");
  ros::NodeHandle n;
  ros::Publisher inmoov_node_center = n.advertise<sensor_msgs::JointState>("/move_group/fake_controller_joint_states", 1000);
  ros::Rate loop_rate(10);
string file = "/home/nixdorf/inMoove_ROS_Project/catkin_ws/src/inmoov_ros/inmoov_controls/scripts/Gestures/CenterPosition.txt";
Stateinput(file);
int count = 0;
while (ros::ok() && count<=jState.name.size())
    {

            inmoov_node_center.publish(jState); //Broadcasts the JointState
            ROS_INFO("Joint %i moved", count); //Displays Data to console
        ++count;

//Calling ros::spinOnce() here is not necessary for this program, because we are not receiving any callbacks.
//However, if you were to add a subscription into this application, and did not have ros::spinOnce() here, your callbacks would never get called.
    ros::spinOnce();
//waits for the next call
    loop_rate.sleep();
  }

  return 0;
}

