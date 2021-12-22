#include <iostream>
#include <vector>

enum Mode
{
	Exit,
	Add,
	Delete,
};

class Data
{
public:
	std::string name;
	double weight;
	double height;

	Data()
		: name("None"), weight(-1.0), height(-1.0) {};
	Data(std::string name_, double weight_, double height_)
		: name(name_), weight(weight_), height(height_) {};
};

int main(int argc, char** argv)
{
	///////////////////////////////////////
	// you can choose which one you want //
	///////////////////////////////////////
	std::vector<Data> data_vec;
	///////////////////////////////////////
	int data_size = 0;
	Data* data = nullptr;
	///////////////////////////////////////





	while (1)
	{
		int mode = 0;
		printf("insert mode(exit->%d, add->%d, delete->%d)->", Mode::Exit, Mode::Add, Mode::Delete);

		std::cin >> mode;

		if (mode == Mode::Exit)
		{
			break;
		}
		else if (mode == Mode::Add)
		{
			printf("insert name->");
			std::string insert_name;
			std::cin >> insert_name;

			printf("insert weight->");
			double insert_weight;
			std::cin >> insert_weight;

			printf("insert height->");
			double insert_height;
			std::cin >> insert_height;

			data_vec.push_back(Data(insert_name, insert_weight, insert_height));
			data_size++;



		}
		else if (mode == Mode::Delete)
		{

			if (data_size == 0)
			{
				printf("Error Empty Data");
			}
			else
			{
				int iter = 0;
				printf("insert name->");
				std::string insert_name;
				std::cin >> insert_name;
				for (int i = 0; i < data_vec.size(); i++)
				{
					if (data_vec[i].name == insert_name)
					{
						iter = i;
						break;
					}
				}
				data_vec.erase(data_vec.begin()+iter);
			}
			data_size--;
		}


		printf("\n----------------------------Data------------------------------\n");
		/*
		if (data != nullptr && data_size != 0)
		{

		}
			for (int i = 0; i < (int)data_size; i++)
				printf("name: [%10s] weight: [%10.1lf], height: [%10.1lf]\n"
					, data[i].name.c_str(), data[i].weight, data[i].height);
		*/
		if (data_vec.size() != 0)
		{
			for (int i = 0; i < (int)data_vec.size(); i++)
				printf("name: [%10s] weight: [%10.1lf], height: [%10.1lf]\n"
					, data_vec[i].name.c_str(), data_vec[i].weight, data_vec[i].height);
		}
		else
		{
			printf("no data");
		}
		printf("--------------------------------------------------------------\n\n");
	}

	return 0;
}
