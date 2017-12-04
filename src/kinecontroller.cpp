#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>

std_msgs::Float32 Wr;
std_msgs::Float32 Wl;
ros::Publisher R_Motor;
ros::Publisher L_Motor; 

float Rr = 0.1 ; // Raio da roda direita
float Rl = 0.1 ; // Raio da roda esquerda
float B = 0.35 ; // Comprimento do eixo
float Red = 1 ;  // Redução

void callback(const geometry_msgs::TwistConstPtr& vel)
{               
        Wr.data = ( (vel->linear.x) + (vel->angular.z)* B/2) * Red / Rr  ;
        Wl.data = ( (vel->linear.x) - (vel->angular.z)* B/2) * Red / Rl ;
        R_Motor.publish(Wr);
        L_Motor.publish(Wl);        
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

    R_Motor = node.advertise<std_msgs::Float32>("/AMR/motorRightSpeed" , 1);   
    L_Motor = node.advertise<std_msgs::Float32>("/AMR/motorLeftSpeed" , 1 );
    ros::Subscriber s = node.subscribe("/cmd_vel",1, callback); 
    ros::spin();
}
