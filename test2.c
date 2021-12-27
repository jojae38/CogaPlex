#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
enum Mode
{
	Exit,
	Add,
	Delete,
};

typedef struct {

	char str[20];
	double weight;
	double height;

}subject;

int main()
{
	int data_size = 0;
	subject* subject_container[200];

	while (1)
	{
		int mode = 0;
		printf("insert mode(exit->%d, add->%d, delete->%d)->", Exit, Add, Delete);
		scanf("%d", &mode);

		if (mode == Exit)
		{
			break;
		}
		else if (mode == Add)
		{
			printf("insert name->");
			char insert_name[20];
			scanf("%s", insert_name);

			printf("insert weight->");
			double insert_weight = 0;
			scanf("%Le", &insert_weight);

			printf("insert height->");
			double insert_height = 0;
			scanf("%Le", &insert_height);

			int iter = 0;
			int match = 0;
			for (int i = 0; i < data_size; i++)
			{
				iter = i;
				match = 1;
				break;
			}
			if (match == 1)
			{
				strcpy(subject_container[iter]->str, insert_name);
				subject_container[iter]->weight = insert_weight;
				subject_container[iter]->height = insert_height;
			}
			else
			{
				subject* sub = malloc(sizeof(subject));
				strcpy(sub->str, insert_name);
				sub->weight = insert_weight;
				sub->height = insert_height;
				subject_container[data_size] = sub;
				data_size++;
			}
		}
		else if (mode == Delete)
		{

			if (data_size == 0)
			{
				printf("Error Empty Data");
			}
			else
			{
				char no_match=0;
				int iter = 0;
				printf("insert name->");
				char insert_name[20];
				scanf("%s", insert_name);
				for (int i = 0; i < data_size; i++)
				{
					if (strcmp(subject_container[i]->str, insert_name) == 0)
					{
						iter = i;
						free(subject_container[iter]);
						no_match = 1;
						break;
					}
				}
				if (no_match == 0)
				{
					printf("Wrong Subject Name Please Try Again!\n");
				}
				else
				{
					for (int i = iter + 1; i < data_size; i++)
					{
						if (subject_container[i] == NULL)
						{
							break;
						}
						else
						{
							subject_container[i - 1] = subject_container[i];
						}
					}
					data_size--;
				}
			}

		}
		printf("\n----------------------------Data------------------------------\n");
		if (data_size != 0)
		{
			for (int i = 0; i < data_size; i++)
				printf("name: [%10s] weight: [%10.1lf], height: [%10.1lf]\n", subject_container[i]->str, subject_container[i]->weight, subject_container[i]->height);
		}
		else
		{
			printf("no data\n");
		}
		printf("--------------------------------------------------------------\n\n");

	}
	for (int i = 0; i < data_size; i++)
	{
		free(subject_container[i]);
	}
	return 0;
}
