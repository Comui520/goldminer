#pragma once
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <Windows.h>
#include <process.h>
#include <mmsystem.h>//包含多媒体设备接口头文件
#pragma comment (lib,"winmm.lib")//加载静态库
#define BUTTONCOUNT 4
#define IMG_COUNT 17
#define BK_COUNT 2
#define pi 3.14159265
#define mine_count 5
#define BUTTONSLEEP 300
enum state {H_swing, H_elongate, H_contract};
struct image 
{
	IMAGE originImage[IMG_COUNT];
	IMAGE maskImage[IMG_COUNT];
	IMAGE bk[BK_COUNT];
};
struct button
{
	int x;
	int y;
	int end_x;
	int end_y;
	char message[100];
};
struct playerOnchart
{
	int rank;
	char name[50];
	int score;
};
bool cmp(playerOnchart a, playerOnchart b)
{	
	return a.score > b.score;
}
struct player
{
	int x;
	int y;
	int w;
	int h;
	int coin;
	char name[50];
	state st;
	float power;
	bool ppt;
};



struct tool
{
	int totem;
	int ppt;//power potion
	int lottery;
	int price[4];
};

struct hook
{
	double leng;
	double x;
	double y;
	double end_x;
	double end_y;
	double angle;
	double swingSpeed;
	bool flag;//左右极限的标识
	double speed;
	double cx;
	double cy;
	bool CD;//记录冷却，即钩子是否完全回来
	state st;
};

typedef struct nodeFormine
{
	int type;
	int width;
	int height;
	int price;
	double x;
	double y;
	double speed;
	bool flag;
	struct nodeFormine* parent;
	struct nodeFormine* next;
	
}nfm;
void deleteNode(nfm* node)
{
	nfm* n = node->next;
	nfm* p = node->parent;
	free (node);
	p->next = n;
	if (n != NULL)
		n->parent = p;
}
FILE* readChart()
{
	
	FILE* f = fopen("./chart/chart.txt", "r");
	if (f == NULL)
	{
		perror("not fount file\n");
	}
	return f;
}
FILE* writeChart()
{
	FILE* f = fopen("./chart/chart.txt", "w");
	if (f == NULL)
	{
		perror("not fount file\n");
	}
	return f;
}
void sleep_us(long long microseconds) {
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq); // 获取计时器频率
	QueryPerformanceCounter(&start);  // 获取当前计数值
	long long ticks_to_wait = (freq.QuadPart * microseconds) / 1000000LL; // 计算需要等待的 ticks
	LARGE_INTEGER now;
	do 
	{
		QueryPerformanceCounter(&now); // 获取当前计数值
	} while ((now.QuadPart - start.QuadPart) < ticks_to_wait); // 等待足够的时间
}