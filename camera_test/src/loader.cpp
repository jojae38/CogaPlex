// #include <ros/ros.h>
// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include "sensor_msgs/CompressedImage.h"

// //#include <image_transport/image_transport.h>
// //#include <opencv2/highgui/highgui.hpp>
// #include <cv_bridge/cv_bridge.h>
#include "camera.hpp"

ros::Subscriber get_file_std;
ros::Publisher pub_data;
std::string img_path_now="/home/cona/Pictures/";
std::string img_path_prev="/home/cona/Pictures/";
cv::Mat img;
void scanCallback(const std_msgs::String::ConstPtr& scan);
int main(int argc, char **argv)
{
    ros::init(argc, argv, "loader_node"); 
    ros::NodeHandle n;
    pub_data = n.advertise<sensor_msgs::CompressedImage>("image_data",10);
    get_file_std =n.subscribe("file_name",10,scanCallback);

    while(ros::ok())
    {
        if(img_path_now!=img_path_prev)
        {
            if(img.empty())
            {
                std::cerr<<"Image load failed!"<<std::endl;
                return -1;
            }
            cv::namedWindow("image");
            cv::moveWindow("image",0,0);
            cv::imshow("image",img);
            img_path_prev=img_path_now;
        }
        cv::waitKey(1);
        ros::spinOnce();
    }
    return 0;
}
void scanCallback(const std_msgs::String::ConstPtr& scan)
{
    img_path_now=scan->data;
    std::cout <<"got picture path"<<std::endl;
    img=cv::imread(img_path_now);
    sensor_msgs::CompressedImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toCompressedImageMsg();
    pub_data.publish(msg);
}
