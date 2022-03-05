// ground_to_space.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<math.h>
#pragma warning(disable:4996)
#define PI 3.14159265358979323846
#define MAXNUM 4
using namespace std;

typedef struct groud_point
{
	double B1, B2, B3;
	double L1, L2, L3;
	double B;
	double L;
	double H;
	double N;
}GP;

typedef struct space_point
{
	long double x;
	long double y;
	long double z;
}SP;

int main()
{
	long double a = 6378137;
	long double f = 1 / 298.257222101;
	long double e = sqrt(1 - pow((long double)(1 - f), 2));

	GP obj[MAXNUM];

	FILE* f1 = fopen("D:\\Code\\General_survey\\ground_to_space\\test.txt", "r");
	for (int i = 0; i < MAXNUM; i++)
	{
		fscanf(f1, "%lf,%lf,%lf  %lf,%lf,%lf  %lf", &obj[i].L1, &obj[i].L2, &obj[i].L3, 
			&obj[i].B1, &obj[i].B2, &obj[i].B3, &obj[i].H);
		obj[i].B = (obj[i].B1 + obj[i].B2 / 60 + obj[i].B3 / 3600) / 180 * PI;
		obj[i].L = (obj[i].L1 + obj[i].L2 / 60 + obj[i].L3 / 3600) / 180 * PI;
		obj[i].N = a / sqrt(1 - pow(e, 2)*pow(sin(obj[i].B), 2));
	}

	SP obj_converted[MAXNUM];

	for (int i = 0; i < MAXNUM; i++)
	{
		obj_converted[i].x = (obj[i].N + obj[i].H)*cos(obj[i].B)*cos(obj[i].L);
		obj_converted[i].y = (obj[i].N + obj[i].H)*cos(obj[i].B)*sin(obj[i].L);
		obj_converted[i].z = (obj[i].N*(1 - pow(e, 2)) + obj[i].H)*sin(obj[i].B);
	}

	FILE* f2 = fopen("D:\\Code\\General_survey\\ground_to_space\\result.txt", "w");
	for (int i = 0; i < MAXNUM; i++)
	{
		fprintf(f2, "%lf,%lf,%lf \n", obj_converted[i].x, obj_converted[i].y, obj_converted[i].z);
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
