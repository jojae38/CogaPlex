#pragma once
#include "ros/ros.h"
#include <iostream>
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
#include <cmath>
#include <time.h>

typedef struct control_node
{
    double x;
    double y;
    double theta;
    double distance;

    double middle_x_pos;
    double middle_y_pos;
    double turtle_x_pos;
    double turtle_y_pos;

    bool near_middle;
    bool near_boundary;
    bool out_boundary;
    bool theta_adjust;
    double rand_theta;

}control_node;

double create_random_theta();
double calc_distance(double x, double y);
double calc_theta_adjust();
void poseCallback(const turtlesim::Pose::ConstPtr&msg);
void check_boundary(int set_Radian,double gap);
void move(double speed, double angle);
void input_speed_and_boundary(double* speed, double* set_Radian);
void sequence(double speed,double set_Radian);