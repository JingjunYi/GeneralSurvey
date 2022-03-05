// space_to_ground.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<math.h>
#pragma warning(disable:4996)
#define PI 3.14159265358979323846
#define MAXNUM 4
using namespace std;

typedef struct space_point
{
	long double x;
	long double y;
	long double z;
}SP;

typedef struct ground_point
{
	long double B1;
	long double B2;
	long double B3;
	long double L1;
	long double L2;
	long double L3;
	long double B;
	long double L;
	long double H;
}GP;



int main()
{
	long double a = 6378137;
	long double f = 1 / 298.257222101;
	long double e = sqrt(1 - pow((double)(1 - f), 2));
	long double b = a * (1 - f);

	FILE* f1 = fopen("D:\\Code\\General_survey\\space_to_ground\\test.txt", "r");
	
	SP obj[MAXNUM];
	for (int i = 0; i < MAXNUM; i++)
	{
		fscanf(f1, "%lf,%lf,%lf", &obj[i].x, &obj[i].y, &obj[i].z);
	}

	GP obj_convented[MAXNUM];
	for (int i = 0; i < MAXNUM; i++)
	{
		obj_convented[i].B = 30;
		obj_convented[i].L = 0;
		obj_convented[i].H = 0;
	}
	
	long double t = 0;

	for (int i = 0; i < MAXNUM; i++)
	{
		if (obj[i].x == 0 && obj[i].y == 0)
		{
			obj_convented[i].L = 0;//caculate L
			
			
			if (obj[i].z > 0)obj_convented[i].B = 90;
			else if (obj[i].z < 0)obj_convented[i].B = -90;//caculate B
		}
		
		else

		{
			while (fabs(t - obj_convented[i].B) >= 1e-10)//caculate B
			{
				t = obj_convented[i].B;
				obj_convented[i].B = atan((obj[i].z + a * pow(e, 2)*sin(t)
					/ sqrt(1 - pow(e, 2)*pow(sin(t), 2)))
					/ sqrt(pow(obj[i].x, 2) + pow(obj[i].y, 2)));
			}

			
			if (obj[i].x != 0 && obj[i].y == 0)//caculate L
			{
				if (obj[i].x > 0) obj_convented[i].L = 0;
				else if (obj[i].x < 0)obj_convented[i].L = 180;
			}
			else if (obj[i].x == 0 && obj[i].y != 0)
			{
				if (obj[i].y > 0)obj_convented[i].L = 90;
				else if (obj[i].y < 0)obj_convented[i].L = -90;
			}

			else if (obj[i].x > 0 && obj[i].y > 0)
			{
				obj_convented[i].L = atan(obj[i].y / obj[i].x);
				obj_convented[i].L = (double)obj_convented[i].L / PI * 180;
			}
			else if (obj[i].x < 0 && obj[i].y>0)
			{
				obj_convented[i].L = atan(obj[i].y / obj[i].x);
				obj_convented[i].L = (double)obj_convented[i].L / PI * 180 + 180;
			}
			else if (obj[i].x < 0 && obj[i].y < 0)
			{
				obj_convented[i].L = atan(obj[i].y / obj[i].x);
				obj_convented[i].L = (double)obj_convented[i].L / PI * 180 - 180;
			}
			else if (obj[i].x > 0 && obj[i].y < 0)
			{
				obj_convented[i].L = atan(obj[i].y / obj[i].x);
				obj_convented[i].L = (double)obj_convented[i].L / PI * 180;
			}
		}
	}

	for (int i = 0; i < MAXNUM; i++)//calculate H
	{
		if (obj_convented[i].B == 90 || obj_convented[i].B == -90)
		{
			if (obj[i].z > 0)obj_convented[i].H = obj[i].z - b;
			else if (obj[i].z < 0)obj_convented[i].H = b - obj[i].z;
		}
		else
		{
			obj_convented[i].H = sqrt(pow(obj[i].x, 2) + pow(obj[i].y, 2)) / cos(obj_convented[i].B)
				- a / sqrt(1 - pow(e, 2)*pow(sin(obj_convented[i].B), 2));
		}
	}

	for (int i = 0; i < MAXNUM; i++)//change B to degree
	{
		obj_convented[i].B = obj_convented[i].B / PI * 180;
	}

	for (int i = 0; i < MAXNUM; i++)//change B,L to degree minute second
	{
		obj_convented[i].B1 = (int)obj_convented[i].B;
		obj_convented[i].B2 = (int)(60 * (obj_convented[i].B - (int)obj_convented[i].B));
		obj_convented[i].B3 = 60 * (60 * (obj_convented[i].B - (int)obj_convented[i].B)
			- (int)(60 * (obj_convented[i].B - (int)obj_convented[i].B)));

		obj_convented[i].L1 = (int)obj_convented[i].L;
		obj_convented[i].L2 = (int)(60 * (obj_convented[i].L - (int)obj_convented[i].L));
		obj_convented[i].L3 = 60 * (60 * (obj_convented[i].L - (int)obj_convented[i].L)
			- (int)(60 * (obj_convented[i].L - (int)obj_convented[i].L)));
	}

	FILE* f2 = fopen("D:\\Code\\General_survey\\space_to_ground\\result.txt", "w");
	for (int i = 0; i < MAXNUM; i++)
	{
		fprintf(f2, "%.0lf°%.0lf′%.7lf″ %.0lf°%.0lf′%.7lf″ %lf\n", 
			obj_convented[i].B1, obj_convented[i].B2, obj_convented[i].B3,
			obj_convented[i].L1, obj_convented[i].L2, obj_convented[i].L3,
			obj_convented[i].H);
	}
		fclose(f1);
		fclose(f2);
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
