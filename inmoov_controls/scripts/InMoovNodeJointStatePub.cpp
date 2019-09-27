#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
///home/nixdorf/inMoove_ROS_Project/catkin_ws/src/inmoov_ros/inmoov_controls/scripts/CenterPosition.txt
using namespace std;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "inmoov_node_pub");

  ros::NodeHandle n;

  ros::Publisher inmoov_node_pub = n.advertise<std_msgs::String>("Project1", 1000);

  ros::Rate loop_rate(10);

  string file = "/home/nixdorf/inMoove_ROS_Project/catkin_ws/src/inmoov_ros/inmoov_controls/scripts/Yes.txt";

  if(ros::ok())
  {
    std_msgs::String msg;

    msg.data = file.c_str();

    ROS_INFO("%s", msg.data.c_str());


    inmoov_node_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
