#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Point.h>
#include <scarab_gazebo/PointArr.h>
#include "planner_to_control/control.h"

int main(int argc, char ** argv){

	ros::init(argc, argv, "sendToControl");
	ros::NodeHandle n;
	ros::Publisher com = n.advertise<planner_to_control::control>("commands", 1000);

	ros::Rate loop_rate(10);

	while(ros::ok()){
		planner_to_control::control msg;

		//test the message for the control node
		msg.turn = 0.4;
		msg.inching = 0.6;

		com.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;
}