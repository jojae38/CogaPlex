#include <iostream>
#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace std;

const string apple_img="/home/cona/image_file/image_apple/";
const string banana_img="/home/cona/image_file/image_banana/";
const string grape_img="/home/cona/image_file/image_grape/";
const string train_image_path="/home/cona/image_file/train_image_set/";
const string test_image_path="/home/cona/image_file/test_image_set/";
const string save_path="/home/cona/image_file/modified_image/";


#define R_pixel (*vec)[train_data_num].pixel_dot[i].R
#define G_pixel (*vec)[train_data_num].pixel_dot[i].G
#define B_pixel (*vec)[train_data_num].pixel_dot[i].B

#define R_cluster (*vec)[train_data_num].cluster_point[j].R
#define G_cluster (*vec)[train_data_num].cluster_point[j].G
#define B_cluster (*vec)[train_data_num].cluster_point[j].B

enum fruit {apple,banana,grape};
enum cluster {empty}; //
typedef struct POINT
{
    double R;
    double G;
    double B;
    int move_val;

}POINT;
typedef struct PIXEL
{
    int x;
    int y;
    int R;
    int G;
    int B; 
    int cluster_val;

}PIXEL;
typedef struct DATA
{
    string file_path;
    uint8_t fruit:2;
    uint8_t data_number;
    int rows;
    int cols;
    vector<PIXEL> pixel_dot;
    // vector<vector<double>> modified_pixel_dot;
    vector<POINT> cluster_point;
}DATA;

class k_means
{
    private:
        int k;
        int train_data_size;
        int train_data_num;
        int test_data_num;
        vector<DATA> trained_data;
        vector<DATA> test_data;
        vector<vector<POINT>> cluster_container;
    public:
    //class init
        k_means();
        k_means(int k_,int data_num_);
        ~k_means();
    //return val
        double return_distance(double x1, double x2, double y1, double y2, double z1, double z2);  
    //return vector

        vector<PIXEL> convert_img_to_vector(cv::Mat *org_img);
        vector<POINT> cluster_point_init();
    //void function
        void add_data(int fruit, int num, vector<DATA>*vec);
        void show_original_pic(int fruit, int pic_num);
        void show_modified_pic(int fruit, int pic_num);

        void re_cluster(vector<DATA>*vec);
        void adjust_cluster(vector<DATA>*vec);

        void convert_and_save(vector<DATA>*vec,int num);



        void do_training();

        void test_picture(int num);
};
k_means::k_means()
{
    k=3;
    train_data_size=90;
    train_data_num=0;
    test_data_num=0;
    
    DATA empty_data={" ",0,0,0,0};
    for(int i=0;i<90;i++)
    {
        trained_data.push_back(empty_data);
    }

    ROS_INFO("Make_k_mean K=3, data_num=90");
}
k_means::k_means(int k_,int data_num_)
{
    k=k_;
    train_data_size=data_num_;
    train_data_num=0;
    test_data_num=0;
    DATA empty_data={" ",0,0,0,0};
    for(int i=0;i<data_num_;i++)
    {
        trained_data.push_back(empty_data);
    }
    ROS_INFO("Make_k_mean K=%d, data_num=%d",k,train_data_size);
}
k_means::~k_means()
{
    ROS_INFO("Delete_k_mean");
}
double k_means::return_distance(double x1, double x2, double y1, double y2, double z1, double z2)
{
    return sqrt(pow(abs(x1-x2),2)+pow(abs(y1-y2),2)+pow(abs(z1-z2),2));
}
vector<PIXEL> k_means::convert_img_to_vector(cv::Mat *org_img)
{
    vector<PIXEL> temp;
    int index=0;
    for(int i=0;i<org_img->rows;i++)
    {
        for(int j=0;j<org_img->cols;j++)
        {
            int R=org_img->at<cv::Vec3b>(i,j)[2];
            int G=org_img->at<cv::Vec3b>(i,j)[1];
            int B=org_img->at<cv::Vec3b>(i,j)[0];
            temp[index].R=R;
            temp[index].G=G;
            temp[index].B=B;
            temp[index].x=j;
            temp[index].y=i;
            temp[index].cluster_val=0;
            index++;
        }
    }
    return temp;
}
vector<POINT> k_means::cluster_point_init()
{
    vector<POINT> temp;
    int val=256/(k+1);
    for(int i=0;i<k;i++)
    {
        temp[i].R=val*i;
        temp[i].G=val*i;
        temp[i].B=val*i;
        temp[i].move_val=0;
    }
    return temp;
}
//fruit 과일 종류, num 과일 번호, 넣을 데이터 벡터
void k_means::add_data(int fruit,int num,vector<DATA>*vec)//사진데이터를 변환해서 벡터에 추가
{
    string file_path;
    string file_name;
    DATA temp_data;
    if(fruit==apple)
    {file_path=apple_img;file_name="apple";}
    else if(fruit==banana)
    {file_path=banana_img;file_name="banana";}
    else if(fruit==grape)
    {file_path=grape_img;file_name="grape";}
    string full_path=file_path+file_name+to_string(num)+".jpeg";
    cv::Mat img=cv::imread(full_path);

    

    // vec init
    
    //k_means::cluster_point_init();
    // for(int i=0;i<50;i++)
    // {
    //     k_means::re_cluster(vec);
    //     k_means::adjust_cluster(vec);
    // }
    
    //K-mean 적용하여 cluster point 를 움직이지 않는 지점까지 이동
    //K-mean 적용된 cluster point 를 저장
    //K-mean 적용하여 변형된 사진을 cv::mat 형태로 저장
    temp_data.fruit=fruit;
    temp_data.file_path=full_path;
    temp_data.data_number=train_data_num;
    temp_data.cols=img.cols;
    temp_data.rows=img.rows;
    temp_data.pixel_dot=convert_img_to_vector(&img);
    //temp_data.cluster_point=;
    (*vec).push_back(temp_data);
    //cluster_container.push_back((*vec)[train_data_num].cluster_point);
    //k_means::convert_and_save(vec,train_data_num);
    train_data_num++;
    ROS_INFO("trained %d",num);
}
void k_means::show_modified_pic(int fruit,int pic_num)
{
    cv::Mat img=cv::imread(trained_data[pic_num].file_path);
    cv::namedWindow("modified_image");
    cv::moveWindow("modified_image",80+img.rows,0);
    cv::imshow("modified_image",img);
}
void k_means::show_original_pic(int fruit,int pic_num)
{
    cv::Mat img=cv::imread(trained_data[pic_num].file_path);
    cv::namedWindow("original_image");
    cv::moveWindow("original_image",80,0);
    cv::imshow("original_image",img);
}
void k_means::re_cluster(vector<DATA>*vec)
{
    for(int i=0;i<(*vec)[train_data_num].pixel_dot.size();i++)
    {
        vector<double> v;
        for(int j=0;j<k;j++)
        {
            v.push_back(k_means::return_distance(R_pixel, R_cluster, G_pixel, G_cluster, B_pixel ,B_cluster));
        }
        int min_index=min_element(v.begin(),v.end())-v.begin();
        (*vec)[train_data_num].pixel_dot[i].cluster_val=min_index;
    }
}
void k_means::adjust_cluster(vector<DATA>*vec)
{
    vector<vector<double>> cluster;
    for(int i=0;i<k;i++)
    {
        vector<double> v={0,0,0,0};//R G B data_num
        cluster.push_back(v);
    }
    for(int i=0;i<(*vec)[train_data_num].pixel_dot.size();i++)
    {
        int cluster_val=(*vec)[train_data_num].pixel_dot[i].cluster_val;
        cluster[cluster_val][3]+=1;
        cluster[cluster_val][0]+=(*vec)[train_data_num].pixel_dot[i].R/255.;
        cluster[cluster_val][1]+=(*vec)[train_data_num].pixel_dot[i].G/255.;
        cluster[cluster_val][2]+=(*vec)[train_data_num].pixel_dot[i].B/255.;
    }
    for(int i=0;i<k;i++)
    {
        (*vec)[train_data_num].cluster_point[i].move_val+=1;
        (*vec)[train_data_num].cluster_point[i].R=cluster[i][0]/cluster[i][3];
        (*vec)[train_data_num].cluster_point[i].G=cluster[i][1]/cluster[i][3];
        (*vec)[train_data_num].cluster_point[i].B=cluster[i][2]/cluster[i][3];
    }

}
void k_means::convert_and_save(vector<DATA>*vec,int num)
{
    int cluster_val=0;
    string full_path=save_path+to_string(num);
    cv::Mat img=cv::imread((*vec)[num].file_path);
    int index=0;
    for(int i=0;i<(*vec)[num].rows;i++)
    {
        for(int j=0;j<(*vec)[num].cols;j++)
        {   
            int cluster_val=(*vec)[num].pixel_dot[i*img.cols+j].cluster_val;
            int cluster_R_val=(*vec)[num].cluster_point[cluster_val].R;
            int cluster_G_val=(*vec)[num].cluster_point[cluster_val].G;
            int cluster_B_val=(*vec)[num].cluster_point[cluster_val].B;
            img.at<cv::Vec3b>(i,j)[2]=cluster_R_val;
            img.at<cv::Vec3b>(i,j)[1]=cluster_G_val;
            img.at<cv::Vec3b>(i,j)[0]=cluster_B_val;
            index++;
        }
    }
    cv::imwrite(full_path,img);
}
void k_means::do_training()
{
    while(true)
    {
        if(train_data_size<=train_data_num)
        {
            break;
        }
        else
        {
            for(int i=1;i<=30;i++)
            {
                add_data(apple,i,&trained_data);
            }
            for(int i=1;i<=30;i++)
            {
                add_data(banana,i,&trained_data);
            }
            for(int i=1;i<=30;i++)
            {
                add_data(grape,i,&trained_data);
            }
        }

    }
}

