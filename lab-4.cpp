

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <iostream>
#include <time.h>

using namespace std;

class Frame
{
public:
	int pageNum;
	bool flag;
};

int main()
{
	srand(time(NULL));
	int process1, process2;
	int i, j, k, t, m, n, r1, r2;
	int temp1, temp2, temp3;
	int diseffect1 = 0;
	int diseffect2 = 0;

	int FIFOnow = 0;
	int LRUnow = 0;
	bool FIFOflag = false;
	bool LRUflag = false;
	float f;

	int inquirySeries[8];

	int myFrameNum = 5;
	Frame MyFrame[5];
	for (i = 0; i < myFrameNum; i++)
		MyFrame[i].pageNum = -1;
	cout << "Frame:" << endl;
	for (i = 0; i < 8; i++)
	{
		inquirySeries[i] = rand() % 8 + 1;
		cout << inquirySeries[i] << " ";
	}
	cout << endl;

	while ((process1 = fork()) == -1);
	if (process1 == 0)
	{
		for (j = 0; j < 8; j++)
		{
			r1 = inquirySeries[j];
			for (int k = 0; k < FIFOnow; ++k)
			{
				if (r1 != MyFrame[k].pageNum)
					continue;
				else
				{
					cout << "Founded !" << endl;
					for (int i = 0; i < FIFOnow; ++i)
					{
						cout << MyFrame[i].pageNum << " ";
					}
					cout << endl;
					FIFOflag = true;
					break;
				}
			}
			if (FIFOflag == false)
			{
				diseffect1++;
				if (FIFOnow < myFrameNum)
				{
					MyFrame[FIFOnow].pageNum = r1;
					MyFrame[FIFOnow].flag = true;
					FIFOnow++;
					for (int i = 0; i < 8; ++i)
					{
						cout << inquirySeries[i] << " ";
					}
					cout << endl;
					cout << "diseffect " << diseffect1 << endl;
					for (int i = 0; i < FIFOnow; ++i)
					{
						cout << MyFrame[i].pageNum << " ";
					}
					cout << endl;
				} else
				{
					temp1 = MyFrame[0].pageNum;
					for (int i = 1; i < 5; ++i)
					{
						MyFrame[i - 1] = MyFrame[i];
					}
					MyFrame[4].pageNum = r1;
					for (int i = 0; i < 8; ++i)
					{
						cout << inquirySeries[i] << " ";
					}
					cout << endl;
					cout << "diseffect " << diseffect1 << endl;
					for (int i = 0; i < FIFOnow; ++i)
					{
						cout << MyFrame[i].pageNum << " ";
					}
					cout << endl;

				}
			}
			FIFOflag = 0;
		}
		f = diseffect1 / 8.0;
		cout << "diseffect FIFO " << f << endl;
		exit(0);
	}
	wait(0);
	for (i = 0; i < myFrameNum; i++)
		MyFrame[i].pageNum = -1;
	while ((process2 = fork()) == -1);
	if (process2 == 0)
	{
		for (m = 0; m < 8; m++)
		{
			r2 = inquirySeries[m];
			for (n = 0; n < LRUnow; n++)
			{
				if (r2 != MyFrame[n].pageNum)
					continue;
				else
				{
					printf("have found!\n");

					for (t = n + 1; t < LRUnow; t++)  //◊Ó≥£∑√Œ µƒ‘⁄∂”ŒÈ◊Óƒ©∂À
						MyFrame[t - 1].pageNum = MyFrame[t].pageNum;
					MyFrame[t - 1].pageNum = r2;
					for (i = 0; i < LRUnow; i++)
						printf("%d ", MyFrame[i].pageNum);
					printf("\n");
					LRUflag = 1;
					break;
				}
			}
			if (LRUflag == 0)
			{
				diseffect2++;
				if (LRUnow < myFrameNum)
				{
					MyFrame[LRUnow].pageNum = r2;
					MyFrame[LRUnow].flag = 'Y';
					LRUnow++;
					for (i = 0; i < 8; i++)printf("%d ", inquirySeries[i]);
					printf("\n");
					printf("diseffect! %d\n", diseffect2);
					for (i = 0; i < LRUnow; i++)
						printf("%d ", MyFrame[i].pageNum);
					printf("\n");
				} else
				{
					temp3 = MyFrame[0].pageNum;
					for (i = 1; i < myFrameNum; i++)
						MyFrame[i - 1].pageNum = MyFrame[i].pageNum;
					MyFrame[myFrameNum - 1].pageNum = r2;
					for (i = 0; i < 8; i++)printf("%d ", inquirySeries[i]);
					printf("\n");
					printf("diseffect %d\n", diseffect2);;
					printf("lose %d\n", temp3);
					for (i = 0; i < LRUnow; i++)
						printf("%d ", MyFrame[i].pageNum);
					printf("\n");
				}
			}
			LRUflag = 0;
		}
		f = diseffect2 / 8.0;
		printf("diseffect rate of LRU %f\n", f);
		exit(0);
	}

}
