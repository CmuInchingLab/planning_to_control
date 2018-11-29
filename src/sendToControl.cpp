#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/Point.h>
#include <scarab_gazebo/PointArr.h>
#include "planner_to_control/control.h"

using namespace std;



class getControlInput
{

public:
	geometry_msgs::Point currpos;
	void getPos(const gazebo_msgs::ModelStates& msg){
		//subscribe to the model states and check x y location 
		currpos = msg.pose[1].position;

	}

	double checkDistance(geometry_msgs::Point curr, geometry_msgs::Point goal){
		
		double dist = sqrt(pow(curr.x-goal.x,2.0)+pow(curr.y-goal.y,2.0));
		return dist;

	}

	void setHeight(geometry_msgs::Point* goal){
		
		//get the height somehow maybe kd tree now? 
		//Or get Andrew's planner class to send and request z for the x,y
		goal->z= 16;

	}

	double getInching(geometry_msgs::Point curr, geometry_msgs::Point goal){
		
		double diff;
		if(goal.z > curr.z){
			diff = goal.z - curr.z;
		}
		else diff = 0; //no inching needed. 

		
		return 2*diff; //some linear proportion of difference

	}
};






int main(int argc, char ** argv){

	ros::init(argc, argv, "sendToControl");
	ros::NodeHandle n;

	//vector<State> plan;
	//need to get plan maybe make a vector or whatever then give me?
	//but I'll make up a message now 
	getControlInput c;

	geometry_msgs::Point goal;
	goal.x = 0.2;
	goal.y = 0.2;
	c.setHeight(&goal);

	//

	

	ros::Publisher com = n.advertise<planner_to_control::control>("commands", 100);
	ros::Subscriber Location = n.subscribe("/gazebo/model_states", 10, &getControlInput::getPos, &c); 
	
	bool nearGoal; 
	ros::Rate loop_rate(10);

	while(ros::ok()){

		planner_to_control::control msg;

		cout << " CURR POSTION Z: " << c.currpos.z <<'\n';
		cout << " GOAL POSTION Z: " << goal.z <<'\n';

		//test the message for the control node

		//get the difference in heights and return inching

		msg.turn = 0.4;
		msg.inching = c.getInching(c.currpos, goal);

		com.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();

	}

	return 0;
}