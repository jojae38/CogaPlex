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
enum fruit {apple,banana,grape};
const char *enum_str[] ={ "apple", "banana", "grape"};
enum cluster_point{empty,A,B,C};

#define R_img   IMG_INFO[i].R_rate
#define G_img   IMG_INFO[i].G_rate
#define B_img   IMG_INFO[i].B_rate
#define R_clu1   CLUSTER_POINT[0].R_cluster
#define G_clu1   CLUSTER_POINT[0].G_cluster
#define B_clu1   CLUSTER_POINT[0].B_cluster
#define R_clu2   CLUSTER_POINT[1].R_cluster
#define G_clu2   CLUSTER_POINT[1].G_cluster
#define B_clu2   CLUSTER_POINT[1].B_cluster
#define R_clu3   CLUSTER_POINT[2].R_cluster
#define G_clu3   CLUSTER_POINT[2].G_cluster
#define B_clu3   CLUSTER_POINT[2].B_cluster
typedef struct FRUIT
{
    int apple;
    int banana;
    int grape;
    int data;
}FRUIT;
typedef struct POINT
{
    double R_cluster_prev;
    double G_cluster_prev;
    double B_cluster_prev;
    double R_cluster;
    double G_cluster;
    double B_cluster;
    int move_val;
    int fruit;
}POINT;
typedef struct IMG_VAL
{
    string filepath;
    int file_num;
    int fruit;
    double R_rate;
    double G_rate;
    double B_rate;
    int rows;
    int cols;
    int cluster_point;
}IMG_VAL;
class k_mean_func
{
private:
    int data_num;
    vector<IMG_VAL> IMG_INFO;
    vector<POINT> CLUSTER_POINT;
public:
    k_mean_func();
    ~k_mean_func();
    void Mat_to_RGB(int fruit,int num);
    void do_training();
    void print_IMG_VAL(int num);
    void find_cluster_point(int iterator);
    void re_cluster();
    void print_cluster_point();
    int closer_cluster_index(double cluster_A_dis, double cluster_B_dis, double cluster_C_dis);
    double return_distance(double x1,double x2,double y1,double y2,double z1,double z2);
    void print_correct_rate();
    void match_cluster_to_fruit();
    void update_prev_cluster();
    bool cluster_point_is_change();
    void test_image(int num);
};
k_mean_func::k_mean_func()
{
    data_num=0;
    for(int i=1;i<=3;i++)
    {
        POINT temp_point;
        temp_point.R_cluster=double((20*i))/255.;
        temp_point.G_cluster=double((20*i))/255.;
        temp_point.B_cluster=double((20*i))/255.;
        temp_point.R_cluster_prev=-1;
        temp_point.G_cluster_prev=-1;
        temp_point.B_cluster_prev=-1;
        temp_point.move_val=0;
        temp_point.fruit=empty;
        CLUSTER_POINT.push_back(temp_point);
    }
    k_mean_func::print_cluster_point();
    ROS_INFO("class k_mean_func activated");
}
k_mean_func::~k_mean_func()
{
    ROS_INFO("class k_mean_func deleted");
}
void k_mean_func::Mat_to_RGB(int fruit,int num)
{

    string file_path;
    string file_name;
    if(fruit==apple)
    {file_path=apple_img;file_name="apple";}
    else if(fruit==banana)
    {file_path=banana_img;file_name="banana";}
    else if(fruit==grape)
    {file_path=grape_img;file_name="grape";}
    string full_path=file_path+file_name+to_string(num)+".jpeg";
    cv::Mat temp_img=cv::imread(full_path);
    //cv::Mat temp_img;
    //cv::GaussianBlur(temp_img2,temp_img,cv::Size(),5);
    IMG_VAL img_val;
    
    img_val.file_num=num;
    img_val.fruit=fruit;
    img_val.filepath=full_path;
    img_val.rows=temp_img.rows;
    img_val.cols=temp_img.cols;
    img_val.cluster_point=empty;

    double R;
    double G;
    double B;
    double T_R=0;
    double T_G=0;
    double T_B=0;
    //filter adjust
    double area_cut=0.25;
    int rough=30;
    int adjust_start_row=temp_img.rows*area_cut;
    int adjust_start_col=temp_img.cols*area_cut;
    int adjust_end_row=temp_img.rows*(1-area_cut);
    int adjust_end_col=temp_img.cols*(1-area_cut);
    int background=0;

    for(int i=adjust_start_row;i<adjust_end_row;i++)
    {
        R=0;
        G=0;
        B=0;
        for(int j=adjust_start_col;j<adjust_end_col;j++)
        {
            int R_temp=temp_img.at<cv::Vec3b>(i,j)[2];
            int G_temp=temp_img.at<cv::Vec3b>(i,j)[1];
            int B_temp=temp_img.at<cv::Vec3b>(i,j)[0];
            if(R_temp<rough&&G_temp<rough&&B_temp<rough)
            {
                background++;
            }
            else if(R_temp>(255-rough)&&G_temp>(255-rough)&&B_temp>(255-rough))
            {
                background++;
            }
            else
            {
                R+=R_temp;
                G+=G_temp;
                B+=B_temp;
            }
        }
        R/=255.;
        G/=255.;
        B/=255.;
        T_R+=R;
        T_G+=G;
        T_B+=B;
    }
    
    double pic_size=double((adjust_end_row-adjust_start_row)*(adjust_end_col-adjust_start_col)-background);
    img_val.R_rate=T_R/pic_size;
    img_val.G_rate=T_G/pic_size;
    img_val.B_rate=T_B/pic_size;
    IMG_INFO.push_back(img_val);
    data_num++;
    ROS_INFO("Trained data : %d",data_num);
}
void k_mean_func::do_training()
{
    for(int i=1;i<=30;i++)
    {
        Mat_to_RGB(apple,i);
    }
    for(int i=1;i<=30;i++)
    {
        Mat_to_RGB(banana,i);
    }
    for(int i=1;i<=30;i++)
    {
        Mat_to_RGB(grape,i);
    }
}
void k_mean_func::print_IMG_VAL(int num)
{
    cout <<"Fruit : ";
    if(IMG_INFO[num].fruit==apple)
        cout << "apple"<<endl;
    else if(IMG_INFO[num].fruit==banana)
        cout << "banana"<<endl;
    else
        cout << "grape"<<endl;
    
    cout << "R_rate : " << IMG_INFO[num].R_rate<<endl;
    cout << "G_rate : " << IMG_INFO[num].G_rate<<endl;
    cout << "B_rate : " << IMG_INFO[num].B_rate<<endl;
    cout << "cluster_point : "<<IMG_INFO[num].cluster_point<<endl;
}
void k_mean_func::re_cluster()
{
    vector<double> A_val_sum={0,0,0,0};//R G B data_num
    vector<double> B_val_sum={0,0,0,0};
    vector<double> C_val_sum={0,0,0,0};

    for(int i=0;i<data_num;i++)
    {
        double dis_A=return_distance(R_img,R_clu1,B_img,B_clu1,G_img,G_clu1);
        double dis_B=return_distance(R_img,R_clu2,B_img,B_clu2,G_img,G_clu2);
        double dis_C=return_distance(R_img,R_clu3,B_img,B_clu3,G_img,G_clu3);

        if(k_mean_func::closer_cluster_index(dis_A,dis_B,dis_C)==A)
        {
            IMG_INFO[i].cluster_point=A;
            A_val_sum[0]+=IMG_INFO[i].R_rate;
            A_val_sum[1]+=IMG_INFO[i].G_rate;
            A_val_sum[2]+=IMG_INFO[i].B_rate;
            A_val_sum[3]++;
        }
        else if(k_mean_func::closer_cluster_index(dis_A,dis_B,dis_C)==B)
        {
            IMG_INFO[i].cluster_point=B;
            B_val_sum[0]+=IMG_INFO[i].R_rate;
            B_val_sum[1]+=IMG_INFO[i].G_rate;
            B_val_sum[2]+=IMG_INFO[i].B_rate;
            B_val_sum[3]++;
        }
        else
        {
            IMG_INFO[i].cluster_point=C;
            C_val_sum[0]+=IMG_INFO[i].R_rate;
            C_val_sum[1]+=IMG_INFO[i].G_rate;
            C_val_sum[2]+=IMG_INFO[i].B_rate;
            C_val_sum[3]++;
        }
    }
    if(A_val_sum[3]!=0)
    {
        CLUSTER_POINT[0].R_cluster=A_val_sum[0]/A_val_sum[3];
        CLUSTER_POINT[0].G_cluster=A_val_sum[1]/A_val_sum[3];
        CLUSTER_POINT[0].B_cluster=A_val_sum[2]/A_val_sum[3];
    }
    CLUSTER_POINT[0].move_val++;
    if(B_val_sum[3]!=0)
    {
        CLUSTER_POINT[1].R_cluster=B_val_sum[0]/B_val_sum[3];
        CLUSTER_POINT[1].G_cluster=B_val_sum[1]/B_val_sum[3];
        CLUSTER_POINT[1].B_cluster=B_val_sum[2]/B_val_sum[3];
    }
    CLUSTER_POINT[1].move_val++;
    if(C_val_sum[3]!=0)
    {
        CLUSTER_POINT[2].R_cluster=C_val_sum[0]/C_val_sum[3];
        CLUSTER_POINT[2].G_cluster=C_val_sum[1]/C_val_sum[3];
        CLUSTER_POINT[2].B_cluster=C_val_sum[2]/C_val_sum[3];
    }
    CLUSTER_POINT[2].move_val++;
    cout << "data_num : "<<CLUSTER_POINT[0].move_val<<endl;
    k_mean_func::print_cluster_point();
}
int k_mean_func::closer_cluster_index(double cluster_A_dis, double cluster_B_dis, double cluster_C_dis)
{
    if(cluster_A_dis<cluster_B_dis)
    {
        if(cluster_A_dis<cluster_C_dis)
            return A;
        else
            return C;

    }
    else if(cluster_C_dis<cluster_B_dis)
    {
        return C;
    }
    else
    {
        return B;
    }
}
double k_mean_func::return_distance(double x1,double x2,double y1,double y2,double z1,double z2)
{
    return sqrt(pow(abs(x1-x2),2)+pow(abs(y1-y2),2)+pow(abs(z1-z2),2));
}
void k_mean_func::find_cluster_point(int iterator)
{
    for(int i=0;i<iterator;i++)
    {
        if(k_mean_func::cluster_point_is_change())
        {
            k_mean_func::update_prev_cluster();
        }
        else
        {
            break;
        }
        k_mean_func::re_cluster();
    }
}
void k_mean_func::print_cluster_point()
{
    for(int i=0;i<3;i++)
    {
        cout << "R_cluster : "<<CLUSTER_POINT[i].R_cluster;
        cout << "  G_cluster : "<<CLUSTER_POINT[i].G_cluster;
        cout << "  B_cluster : "<<CLUSTER_POINT[i].B_cluster<<endl;
    }
}
void k_mean_func::print_correct_rate()
{
    int correct=0;
    for(int i=0;i<data_num;i++)
    {
        if((i)%30==0)
            cout <<endl;
        cout <<IMG_INFO[i].cluster_point;
    }
    cout <<endl;
}
void k_mean_func::match_cluster_to_fruit()
{
    FRUIT clusterA={0,0,0,0};
    FRUIT clusterB={0,0,0,0};
    FRUIT clusterC={0,0,0,0};
    for(int i=0;i<data_num;i++)
    {
        if(IMG_INFO[i].cluster_point==1)
        {
            clusterA.data++;
            if(IMG_INFO[i].fruit==apple)
                clusterA.apple++;
            else if(IMG_INFO[i].fruit==banana)
                clusterA.banana++;
            else
                clusterA.grape++;
        }
        else if(IMG_INFO[i].cluster_point==2)
        {
            clusterB.data++;
            if(IMG_INFO[i].fruit==apple)
                clusterB.apple++;
            else if(IMG_INFO[i].fruit==banana)
                clusterB.banana++;
            else
                clusterB.grape++;
        }
        else
        {
            clusterC.data++;
            if(IMG_INFO[i].fruit==apple)
                clusterC.apple++;
            else if(IMG_INFO[i].fruit==banana)
                clusterC.banana++;
            else
                clusterC.grape++;
        }
    }
    double A_acc;
    double B_acc;
    double C_acc;
    if(clusterA.apple>=(clusterA.data+1)/2)
    {
        CLUSTER_POINT[0].fruit=apple;
        A_acc=double(clusterA.apple)/double(clusterA.data);
    }
    else if(clusterA.banana>=(clusterA.data+1)/2)
    {
        CLUSTER_POINT[0].fruit=banana;
        A_acc=double(clusterA.banana)/double(clusterA.data);
    }
    else
    {
        CLUSTER_POINT[0].fruit=grape;
        A_acc=double(clusterA.grape)/double(clusterA.data);
    }

    if(clusterB.apple>=(clusterB.data+1)/2)
    {
        CLUSTER_POINT[1].fruit=apple;
        B_acc=double(clusterB.apple)/double(clusterB.data);
    }
    else if(clusterB.banana>=(clusterB.data+1)/2)
    {
        CLUSTER_POINT[1].fruit=banana;
        B_acc=double(clusterB.banana)/double(clusterB.data);
    }
    else
    {
        CLUSTER_POINT[1].fruit=grape;
        B_acc=double(clusterB.grape)/double(clusterB.data);
    }

    if(clusterC.apple>=(clusterC.data+1)/2)
    {
        CLUSTER_POINT[2].fruit=apple;
        C_acc=double(clusterC.apple)/double(clusterC.data);
    }
    else if(clusterC.banana>=(clusterC.data+1)/2)
    {
        CLUSTER_POINT[2].fruit=banana;
        C_acc=double(clusterC.banana)/double(clusterC.data);
    }
    else
    {
        CLUSTER_POINT[2].fruit=grape;
        C_acc=double(clusterC.grape)/double(clusterC.data);
    }
    
    cout << "cluster A point : " << enum_str[CLUSTER_POINT[0].fruit]<<" Accuracy : "<< A_acc <<endl;
    cout << "cluster B point : " << enum_str[CLUSTER_POINT[1].fruit]<<" Accuracy : "<< B_acc <<endl;
    cout << "cluster C point : " << enum_str[CLUSTER_POINT[2].fruit]<<" Accuracy : "<< C_acc <<endl;
}
void k_mean_func::update_prev_cluster()
{
    for(int i=0;i<3;i++)
    {
        CLUSTER_POINT[i].R_cluster_prev=CLUSTER_POINT[i].R_cluster;
        CLUSTER_POINT[i].G_cluster_prev=CLUSTER_POINT[i].G_cluster;
        CLUSTER_POINT[i].B_cluster_prev=CLUSTER_POINT[i].B_cluster;
    }
}
bool k_mean_func::cluster_point_is_change()
{
    for(int i=0;i<3;i++)
    {
        if(CLUSTER_POINT[i].R_cluster_prev!=CLUSTER_POINT[i].R_cluster)
        return true;
        if(CLUSTER_POINT[i].G_cluster_prev!=CLUSTER_POINT[i].G_cluster)
        return true;
        if(CLUSTER_POINT[i].B_cluster_prev!=CLUSTER_POINT[i].B_cluster)
        return true;
    }
    return false;
}
void k_mean_func::test_image(int num)
{
    string full_path=test_image_path+to_string(num)+".jpeg";
    cv::Mat temp_img=cv::imread(full_path);

    IMG_VAL img_val;
    
    img_val.file_num=num;
    img_val.cluster_point=empty;

    double R;
    double G;
    double B;
    double T_R=0;
    double T_G=0;
    double T_B=0;
    //filter adjust
    double area_cut=0.25;
    int rough=30;
    int adjust_start_row=temp_img.rows*area_cut;
    int adjust_start_col=temp_img.cols*area_cut;
    int adjust_end_row=temp_img.rows*(1-area_cut);
    int adjust_end_col=temp_img.cols*(1-area_cut);
    int background=0;

    for(int i=adjust_start_row;i<adjust_end_row;i++)
    {
        R=0;
        G=0;
        B=0;
        for(int j=adjust_start_col;j<adjust_end_col;j++)
        {
            int R_temp=temp_img.at<cv::Vec3b>(i,j)[2];
            int G_temp=temp_img.at<cv::Vec3b>(i,j)[1];
            int B_temp=temp_img.at<cv::Vec3b>(i,j)[0];
            if(R_temp<rough&&G_temp<rough&&B_temp<rough)
            {
                background++;
            }
            else if(R_temp>(255-rough)&&G_temp>(255-rough)&&B_temp>(255-rough))
            {
                background++;
            }
            else
            {
                R+=R_temp;
                G+=G_temp;
                B+=B_temp;
            }
        }
        R/=255.;
        G/=255.;
        B/=255.;
        T_R+=R;
        T_G+=G;
        T_B+=B;
    }
    
    double pic_size=double((adjust_end_row-adjust_start_row)*(adjust_end_col-adjust_start_col)-background);
    img_val.R_rate=T_R/pic_size;
    img_val.G_rate=T_G/pic_size;
    img_val.B_rate=T_B/pic_size;

    double dis_A = return_distance(R_clu1,img_val.R_rate,G_clu1,img_val.G_rate,B_clu1,img_val.B_rate);
    double dis_B = return_distance(R_clu2,img_val.R_rate,G_clu2,img_val.G_rate,B_clu2,img_val.B_rate);
    double dis_C = return_distance(R_clu3,img_val.R_rate,G_clu3,img_val.G_rate,B_clu3,img_val.B_rate);

    int index=k_mean_func::closer_cluster_index(dis_A,dis_B,dis_C);

    cout << "I thick this fruit is ["<<enum_str[CLUSTER_POINT[index-1].fruit]<<"]"<<endl;
}