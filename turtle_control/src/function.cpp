#include "function.hpp"

const double x_min=0.0;
const double y_min=0.0;
const double x_max=11.0;
const double y_max=11.0;
double x_middle=5.54;
double y_middle=5.54;
const double PI=3.14159265359;
extern control_node pose_data;
extern ros::Publisher turtle_Publisher;
extern ros::Subscriber pose_subscriber;
extern turtlesim::Pose turtlesim_pose;

double create_random_theta()
{
    double rand_theta;
    rand_theta=rand()%360;
    rand_theta=rand_theta/360.0;
    rand_theta=(rand_theta-0.5)*2*PI;
    return rand_theta;
}

double calc_distance(double x, double y)
{
    return sqrt(pow((x-x_middle),2)+pow((y-y_middle),2));
}
double calc_theta_adjust()
{
    double match;
    if(pose_data.near_middle==true)
    {
        //match=pose_data.rand_theta/pose_data.theta;
    }
    else
    {
        pose_data.middle_x_pos=(x_middle-pose_data.x)/pose_data.distance;
        pose_data.middle_y_pos=(y_middle-pose_data.y)/pose_data.distance;
        pose_data.turtle_x_pos=cos(pose_data.theta);
        pose_data.turtle_y_pos=sin(pose_data.theta);
        match=pose_data.middle_x_pos*pose_data.turtle_x_pos+pose_data.middle_y_pos*pose_data.turtle_y_pos;
    }
    //cout <<pose_data.rand_theta<<"  "<<pose_data.theta<<endl;
    //cout <<match<<endl;
    
    return match;
}
void poseCallback(const turtlesim::Pose::ConstPtr&msg)
{
    pose_data.x=msg->x;
    pose_data.y=msg->y;
    pose_data.theta=msg->theta;
}
void check_boundary(int set_Radian,double gap)
{
    pose_data.distance=calc_distance(pose_data.x,pose_data.y);
    if(pose_data.distance<=1)
        pose_data.near_middle=true;
    else
        pose_data.near_middle=false;
    if(pose_data.distance+gap>=set_Radian||x_max<=pose_data.x+gap||x_min>=pose_data.x-gap||y_max<=pose_data.y+gap||y_min>=pose_data.y-gap)
        pose_data.near_boundary=true;
    else
        pose_data.near_boundary=false;
    if(pose_data.distance+0.1>=set_Radian)
        pose_data.out_boundary=true;
    else
        pose_data.out_boundary=false;
    if(calc_theta_adjust()>=0.8)
        pose_data.theta_adjust=true;
    else
        pose_data.theta_adjust=false; 
}
void sequence(double speed,double set_Radian)
{
    double move_speed;
    double angle;
    check_boundary(set_Radian,speed);
    if(pose_data.out_boundary==true||pose_data.near_middle==true)
    {
        if(pose_data.theta_adjust==false)
        {   
            move_speed=0;
            angle=1;
        }
        else
        {
            move_speed=speed;
            angle=0;
            pose_data.rand_theta=create_random_theta();
        }
    }
    else if(pose_data.near_boundary==true)
    {
        if(pose_data.theta_adjust==true)
        {
            move_speed=speed;
            angle=0;
        }
        else
        {
            move_speed=speed;
            angle=3.14;
        }
    }
    else
    {
        move_speed=speed;
        angle=0;
    }
    move(move_speed,angle);
}
void move(double speed, double angle)
{
    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x=speed;
    vel_msg.linear.y=0;
    vel_msg.linear.z=0;

    vel_msg.angular.x=0;
    vel_msg.angular.y=0;
    vel_msg.angular.z=angle;
    turtle_Publisher.publish(vel_msg);
    std::cout << pose_data.distance<<std::endl;
}
void input_speed_and_boundary(double* speed, double* set_Radian)
{
    std::cout << "Set speed(1,2,3): ";
    std::cin>>*speed;
    std::cout << "Set Radian(Max=7, Min =1): ";
    std::cin>>*set_Radian;
    if(*speed ==2)
        *speed=2;
    else if(*speed ==3)
        *speed=3;
    else
        *speed=1;

    if(*set_Radian>7&&*set_Radian<1)
        *set_Radian=7;
}