
#include "camera.hpp"
ros::Publisher send_picture_path;
int main(int argc,char**argv)
{
    std::string filename="robot_model.jpeg";
    std::string path="/home/cona/Pictures/";
    std::string full_path=path+filename;
    ros::init(argc,argv,"send_picture_path");
    ros::NodeHandle n;
    send_picture_path = n.advertise<std_msgs::String>("file_name",1);
    
    while(ros::ok())
    {
        std::cin>>filename;
        if(filename=="exit")
            break;
        full_path=path+filename;
        std::cout <<"Looking for file: " <<path<<filename<<"\n";
        int exist = access(full_path.c_str(),F_OK);
        if(exist<0)
        {
            std::cout <<"no such file in "<< path << " <--directory"<<std::endl;
            std::cout <<"please try again"<<std::endl;
        }
        else
        {
            ROS_INFO("Publishing Picture %s path [%s] as [file_name] topic",filename.c_str(),full_path.c_str());
            std_msgs::String std;
            std.data=full_path;
            send_picture_path.publish(std);
        }
    }
    return 0;
}