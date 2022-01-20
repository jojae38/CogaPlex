#include "function.hpp"
using namespace std;

ros::Publisher turtle_Publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;

control_node pose_data;

int main(int argc , char** argv)
{
    ros::init(argc,argv,"control_node");
    
    ros::NodeHandle n;
    ros:: Rate loop_Rate(5);
    turtle_Publisher=n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",100);
    pose_subscriber=n.subscribe("turtle1/pose",100,poseCallback);

    srand(time(NULL));
    double speed =1.0;
    double angle =0.0;
    double set_Radian=7.0;

    input_speed_and_boundary(&speed,&set_Radian);
    pose_data.rand_theta=create_random_theta();
    while(ros::ok())
    {
        sequence(speed,set_Radian);
        ros::spinOnce();
        loop_Rate.sleep();
    }
    return 0;
}
