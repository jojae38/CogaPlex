#include "k_mean_picture.hpp"
#include "k_mean_server/print_result.h"
k_mean_func k_mean_func_;
bool add(k_mean_server::print_result::Request  &req,k_mean_server::print_result::Response &res)
{
  ROS_INFO("GOT A REQUEST!");
  for(int i=0;i<30;i++)
  {
      string temp="test image ["+to_string(i+1)+ "] this fruit is ["+k_mean_func_.test_image(i+1)+"]";
      res.Result.push_back(temp);
  }
  return true;
}
int main(int argc,char ** argv)
{
    
    ros::init(argc,argv,"server_node");
    ros::NodeHandle n;
    k_mean_func_.do_training();
    k_mean_func_.find_cluster_point(100);
    k_mean_func_.match_cluster_to_fruit();

    ros::ServiceServer service = n.advertiseService("print_result", add);
    ROS_INFO("Ready to post Fruit Result");
    ros::spin();

    return 0;
}