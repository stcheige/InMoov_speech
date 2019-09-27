/*! ROS::NodeJointStateSubNPub */

#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/String.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//Folder where all .py gestures are saved
string gesturePathPy ="/home/nixdorf/inMoove_ROS_Project/catkin_ws/src/inmoov_ros/inmoov_controls/scripts/MyRobotLabGestures/Gestures/Gesture without Text/";

//!  Joint State Publisher Class
/*!
  Reads Files and converts them to a ROS::JointState and publishes them on the "/move_group/fake_controller_joint_states" topic
*/

class StatePub{
public:
    //! Constructor
    /*!
      Needs a filepath and a case to switch between filetypes.
      Python files are from MyRobotLab and the txt files contain the information like the JointStatePublisher.
    */
    StatePub(string file, int gestureCase);
    void pub(); //!< Publisher funtion
    void gestureConverter(string file); //!< Converter funciton
    float converter(float x, float maxIn, float minIn, float maxOut, float minOut); //!< Scaling funciton
private:
    sensor_msgs::JointState jState; /*!< the JointState */
    ros::Publisher inmoov_node_sub_n_pub; /*!< the Publisher */
};

void StatePub::gestureConverter(string file){
    //! Reads in the scaling factors from the Convertion.txt and the values from the .py file and writes them in the jState
    /*!
      \param file the file path from the gesture
    */
    //Struct to save all values for convertion
    struct Convertion{
      string name;
      //Array of [0]min/max of JState [1]min/max of skeletonConfig
      float min[2]={};
      float max[2]={};
    } Convertions[28];
     ifstream in("Convertion.txt");
     for(unsigned int i = 0; i<28; i++){
         in>>Convertions[i].name;
         in>>Convertions[i].min[0];
         in>>Convertions[i].min[1];
         in>>Convertions[i].max[0];
         in>>Convertions[i].max[1];
     }
     in.close();
    in.open(file.c_str());
    string s;
    while (in.eof() == false) {
            getline(in, s);
    if (s.find("move") != string::npos) {
                    //loops as long as it finds a value
        if (s.find("Head") != string::npos) {
            int head[2];
            s = s.substr(s.find('(')+1);
            sscanf_s(s.c_str(),"%d, %d)", &head[0], &head[1]);
            for(int i = 5; i <= 6; i++){
                jState.name.push_back (Convertions[i].name);
                jState.position.push_back (converter(head[i-5],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
            }
                    }
        if (s.find("Arm") != string::npos) {
                if (s.find("left") != string::npos) {
                    int h[4];
                    s = s.substr(s.find(',')+1);
                    sscanf_s(s.c_str(),"%d,%d,%d,%d)", &h[0], &h[1], &h[2], &h[3]);
                    for(int i = 18; i <= 22; i++){
                        jState.name.push_back (Convertions[i].name);
                        jState.position.push_back (converter(h[i-18],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
                    }
                }
                else{
                    int h[4];
                     s = s.substr(s.find(',')+1);
                    sscanf_s(s.c_str(),"%d,%d,%d,%d)", &h[0], &h[1], &h[2], &h[3]);
                    for(int i = 8; i <= 12; i++){
                        jState.name.push_back (Convertions[i].name);
                        jState.position.push_back (converter(h[i-8],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
                    }
                }
                    }
        if (s.find("Hand") != string::npos) {
                if (s.find("left") != string::npos) {
                    int h[6];
                    s = s.substr(s.find(',')+1);
                    sscanf_s(s.c_str(),"%d,%d,%d,%d,%d,%d)", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5]);
                    for(int i = 22; i < 28; i++){
                        jState.name.push_back (Convertions[i].name);
                        jState.position.push_back (converter(h[i-22],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
                    }
                }
                else {
                    int h[6];
                    s = s.substr(s.find(',')+1);
                    sscanf_s(s.c_str(),"%d,%d,%d,%d,%d,%d)", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5]);
                    for(int i = 12; i < 18; i++){
                        jState.name.push_back (Convertions[i].name);
                        jState.position.push_back (converter(h[i-12],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
                    }
                }
                    }
        if (s.find("Torso") != string::npos) {
            int h[2];
            s = s.substr(s.find('(')+1);
            sscanf_s(s.c_str(),"%d,%d,90)", &h[0], &h[1]);
            for(int i = 0; i <= 2; i++){
                jState.name.push_back (Convertions[i].name);
                jState.position.push_back (converter(h[i-5],Convertions[i].max[1], Convertions[i].min[1],Convertions[i].max[0], Convertions[i].min[0]));
            }
        }
    }
}
    in.close();
}

float StatePub::converter(float x, float maxIn, float minIn, float maxOut, float minOut){
    //! gets a value and four scaling factors to convert the value on a new scale
    /*!
      \param x the input value
      \param maxIn the maximum of the input scale
      \param minIn the minimum of the input scale
      \param maxOut the maximum of the output scale
      \param minOut the minimum of the output scale
    */
    //x is the value from python gestures
    //maxIn/minIn are the values from the skeleton config files
    //maxOut/minOut are the values from the JointstatePublisher
    float y = (x-minIn)*((maxOut-minOut)/(maxIn-minIn))+minOut;
    return y;
}

StatePub::StatePub(string file, int gestureCase){
    /*!
      \param file the file path from the gesture
      \param gestureCase switch variable for the gesture type
    */
    ros::NodeHandle r;
    inmoov_node_sub_n_pub = r.advertise<sensor_msgs::JointState>("/move_group/fake_controller_joint_states", 1000);
    //get JointState Data from File
    switch (gestureCase) {
    case 0:
        ifstream in(file.c_str());
        string s;
        float f;
        while(in.eof() ==false){
            in>> s;
            in>> f;
            jState.name.push_back (s);
            jState.position.push_back (f);
            //ROS_INFO("%s %f", s.c_str(), f);
        }
        in.close();
        break;
    case 1:
       gestureConverter(file);
       break;
    default:
        break;
    }
}


void StatePub::pub(){
    //! Publishes the JointState with name and position
    for(int i = 0; i<=jState.name.size(); ++i){
        inmoov_node_sub_n_pub.publish(jState); //Broadcasts the JointState
        string s = jState.name[i];
        float f = jState.position[i];
        if(i<jState.name.size())
        ROS_INFO("%s moved to %f", s.c_str(), f);
    }
}

void JointStreamSub(const std_msgs::String::ConstPtr& msg){
    //! the subscriber part which listens to given topic and checks file type
    /*!
    \param msg the name from the gesture which was published by chatterbot
    */
    string s = msg->data;
    if(s.find("()") != nullptr){
    s = s[0,end-2];
    s = gesturePath + s + ".py";
    StatePub a(s,1);
    } else StatePub a(s,0);

    a.pub();
}

int main(int argc, char **argv)
{
    //! main function which initalizes the node
    //! "gestOut" is the topic it subscribes to
//Initialization for the node
  ros::init(argc, argv, "inmoov_sub_n_pub");
  ros::NodeHandle n;
  ros::Subscriber inmoov_node_sub = n.subscribe("gestOUT", 1000, JointStreamSub);
  ros::Rate loop_rate(10);

    while (ros::ok())
    {
//Displays Data to console
  //  ROS_INFO("CenterPositionRunning");
//Calling ros::spinOnce() here is not necessary for this program, because we are not receiving any callbacks.
//However, if you were to add a subscription into this application, and did not have ros::spinOnce() here, your callbacks would never get called.
    ros::spinOnce();
//waits for the next call
    loop_rate.sleep();
  }

  return 0;
}

