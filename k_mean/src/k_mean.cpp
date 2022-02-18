
#include "k_mean.hpp"
using namespace std;

int main(int argc ,char** argv)
{   
    ros::init(argc, argv, "k_mean_node"); 
    int K=0;
    int order=0;
    int pic_num=0;

    cout <<"Type in K Value: "<<endl;
    cin >>K;
    k_means k_means_(K,90);//training 후 complete message 출력
    cout <<sizeof(k_means_)<<endl;
    k_means_.do_training();
    while(true)
    {
        cout <<"______________________________________"<<endl;
        cout <<"_______________function_______________"<<endl;
        cout <<"1.Show apple picture------------------"<<endl;
        cout <<"2.Show banana picture-----------------"<<endl;
        cout <<"3.Show grape picture------------------"<<endl;
        cout <<"4.test picture : what is this fruit?--"<<endl;
        cout <<"5.exit--------------------------------"<<endl;
        cout <<"______________________________________"<<endl;

        cin>>order;
        if(int(order)>5&&int(order)<0)
        {
            cout <<"please type again"<<endl;
        }
        else
        {
            if(order==1||order==2||order==3)
            {
                cout <<"which one? (under 30)"<<endl;
                cin>>pic_num;
                pic_num/=30;//데이터가 총 30개라...
                k_means_.show_original_pic(order-1,pic_num);//원본 사진 출력 0번 사과 1번 바나나 2번 포도
                k_means_.show_modified_pic(order-1,pic_num);//바뀐 사진 출력 0번 사과 1번 바나나 2번 포도
                cv::waitKey(10000);
                //해당 번호의 K_mean된 과일 사진 출력 
            }
            else if(order==4)
            {
                
                
                //축적된 데이터를 기반으로 판단
            }
            else
            {break;}
        }
    }
    return 0;
}