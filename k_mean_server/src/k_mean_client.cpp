#include "k_mean_picture.hpp"
#include "k_mean_server/print_result.h"

int main(int argc,char ** argv)
{
    ros::init(argc,argv,"client_node");
    ros::NodeHandle n;

    ros::ServiceClient client = n.serviceClient<k_mean_server::print_result>("print_result");
    k_mean_server::print_result srv;
    if (client.call(srv))
    {
        for(int i=0;i<30;i++)
        {
            cout<<srv.response.Result[i]<<endl;
        }
        ROS_INFO("Result Post Completed!");
    }
    else
    {
        ROS_ERROR("Failed to call Fruit Result");
        return 1;
    }
    return 0;
}