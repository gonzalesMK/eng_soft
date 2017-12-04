#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <math.h>

ros::Publisher goal_pub;

float X,Y;

float goal_x, goal_y;

geometry_msgs::PoseStamped start, goal;
nav_msgs::Odometry odom;

bool has_goal= false;
bool returning = false;
bool stop = false;

void stop_callback(const std_msgs::Bool stop_msg)
{
    stop = stop_msg.data;
}


void callback(const geometry_msgs::PoseStamped pose)
{               
    goal = pose;
    goal_x = pose.pose.position.x;
    goal_y = pose.pose.position.y;

    has_goal = true;
    ROS_INFO ("SENDIND GOAL");
    goal_pub.publish(goal);
}


void get_odom(const nav_msgs::Odometry odom_msg){
    odom = odom_msg;

    X = odom_msg.pose.pose.position.x;
    Y = odom_msg.pose.pose.position.y;

}

int main( int argc, char **argv)
{
    ros::init(argc,argv, "Kinect_Control");
    
    ros::NodeHandle node;
    
    /*  To get the constants using parameters
     * node.getParam( "Rl", Rl);
     * node.getParam( "Rr", Rr);
     * node.getParam( "B", B);
     * node.param( "Red", Red, 1);
     */

    start.header.frame_id = "world";
    start.pose.position.x = 3.34732818604;
    start.pose.position.y = 0.762228190899;
    start.pose.orientation.x = 2.4482731078 * pow(10,-06);
    start.pose.orientation.y = 5.23803646502 * pow(10,-06);
    start.pose.orientation.z = -0.0711686983705;
    start.pose.orientation.w = 0.997464299202;

    goal_pub = node.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal" , 1);

    ros::Subscriber goal_node = node.subscribe("/goal",1, callback);
    ros::Subscriber stop_node = node.subscribe("/stop",1, stop_callback);
    ros::Subscriber odom_node = node.subscribe("/odom",1, get_odom);

    while(ros::ok()){

        ros::spinOnce();
        double dist=100;

        if(stop){
            goal.pose = odom.pose.pose;
            goal_pub.publish(goal);
            returning = false;
            has_goal = false;
            ROS_ERROR("STOPPED!");
            stop = false;
        }


        if( has_goal ){

            dist = pow(X - goal_x,2) + pow(Y - goal_y, 2);

            if( dist < 0.01){

                if( returning ){
                    has_goal = false;
                    returning = false;
                } else {
                    goal_x = start.pose.position.x;
                    goal_y = start.pose.position.y;
                    goal = start ;
                    ROS_INFO ("RETURNING");
                    ros::Duration(1).sleep();
                    goal_pub.publish(goal);
                    returning = true;
                }
            }


        }

    }
}
