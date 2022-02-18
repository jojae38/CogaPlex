#include "k_mean_picture.hpp"
using namespace std;

int main()
{
    int num;
    int iterator;
   k_mean_func k_mean_func_;
   k_mean_func_;
   k_mean_func_.do_training();
    cout <<"Iteration_val: ";
    cin >> iterator;

    k_mean_func_.find_cluster_point(iterator);
    k_mean_func_.match_cluster_to_fruit();

   while(true)
   {
       cout << "Type witch picture want to test? (num): ";
       cin >>num;
       if(num>=30)
           break;

        k_mean_func_.test_image(num);
       //k_mean_func_.print_IMG_VAL(num);
   }
   k_mean_func_.print_correct_rate();
    return 0;
}