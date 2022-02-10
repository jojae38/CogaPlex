#include "camera.hpp"
ros::Publisher camera_frame;

int main (int argc, char** argv){
	ros::init(argc, argv, "cv_bridge_test");
	ros::NodeHandle n;
	camera_frame=n.advertise<sensor_msgs::CompressedImage>("image_data",100);
	cv::VideoCapture cap(0);
	if(!cap.isOpened())
	{
		std::cerr<<"Camera open failed!"<<std::endl;
		return -1;
	}	
	cv::Mat frame;
	while(true)
	{
		cap>>frame;
		if(frame.empty())
			break;

		imshow("frame",frame);
		sensor_msgs::CompressedImagePtr frame_msg=cv_bridge::CvImage(std_msgs::Header(),"bgr8",frame).toCompressedImageMsg();
		camera_frame.publish(frame_msg);
		if(cv::waitKey(10)==27)
		break;
	}
	cv::destroyAllWindows();
	return 0;
}