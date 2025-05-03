#pragma once
#include "common.h"
#include "BGM.h"
void drawPlayer(const image *img, const player* p, const hook h)
{
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(0, 0, getwidth(), getheight());
	putimage(0, p->y + p->h, &img->bk[0]);
	//正常状态
	if (p->st == H_swing || p->st == H_contract)
	{
		putimage(p->x, p->y, &img->maskImage[6], SRCAND);
		putimage(p->x, p->y, &img->originImage[6], SRCPAINT);
	}
	//开始推
	else if (p->st == H_elongate)
	{
		putimage(p->x, p->y, &img->maskImage[7], SRCAND);
		putimage(p->x, p->y, &img->originImage[7], SRCPAINT);
	}
	if (p->ppt)
	{
		putimage(497, 77, &img->maskImage[13], SRCAND);
		putimage(497, 77, &img->originImage[13], SRCPAINT);
	}
	setlinestyle(PS_SOLID, 4);
	setlinecolor(RGB(28, 290, 290));
	line(h.x, h.y, h.end_x, h.end_y);
	line(h.end_x, h.end_y, h.end_x + sin((3 * pi) / 4 - h.angle) * 30, h.end_y - cos((3 * pi) / 4 - h.angle) * 30);
	line(h.end_x, h.end_y, h.end_x + sin(h.angle - pi / 4) * 30, h.end_y + cos(h.angle - pi / 4) * 30);

}

//void drawPlayermove(IMAGE img[], player* p)
//{
//	setfillcolor(RGB(255, 215, 0));
//	BeginBatchDraw();
//	solidrectangle(p->x, p->y, p->x + 140, p->y + 120);
//	putimage(p->x, p->y, &img[6 + 7], SRCAND);
//	putimage(p->x, p->y, &img[6], SRCPAINT);
//	FlushBatchDraw();
//}

void drawMine(nfm* head, image *img)
{
	nfm* p = head->next;//p为工作指针
	while (p != NULL)
	{
		putimage(p->x, p->y, &img->maskImage[p->type], SRCAND);
		putimage(p->x, p->y, &img->originImage[p->type], SRCPAINT);
		p = p->next;
	}
}
void drawButton(int x, int y, int ex, int ey, char message[])
{
	settextcolor(BLACK);
	setfillcolor(WHITE);
	solidrectangle(x, y, ex, ey);
	if (strlen(message) > 0)
	{
		RECT r{ x, y, ex, ey };
		settextcolor(BLACK);//文字颜色
		settextstyle(ey - y, 25, "微软雅黑");
		setbkmode(TRANSPARENT);//不会覆盖文字后面的图形（透明）
		drawtext(message, &r, DT_CENTER);
		
	}
}
void menu(button B[])
{
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(0, 0, getwidth(), getheight());
	for (int i = 0; i < BUTTONCOUNT; i++)
		drawButton(B[i].x, B[i].y, B[i].end_x, B[i].end_y, B[i].message);
	RECT re{ 10, 50, getwidth() - 10, 350 };
	settextstyle(200, 100, "微软雅黑");
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(255, 215, 0));
	drawtext("黄金矿工", &re, DT_CENTER);
}
void showChart()
{
	playerOnchart p[11];
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(0, 0, getwidth(), getheight());
	FILE* f = readChart();
	char str[10][500] = { " " };
	int i = 0;
	while (!feof(f) && i < 10)
	{
		fscanf(f, "%d %s %d", &p[i].rank, p[i].name, &p[i].score);			
		sprintf(str[i], "%d %s %d", p[i].rank, p[i].name, p[i].score);
		i++;
	}
	fclose(f);
	while (1)
	{
		BeginBatchDraw();
		for (int j = 0; j < 10; j++)
		{
			drawButton(200, j * (getheight() / 10) + 10, getwidth() - 200, (j + 1) * (getheight() / 10) - 10, str[j]);
			ExMessage mouse;
			peekmessage(&mouse);
			if (mouse.message == WM_LBUTTONDOWN || mouse.message == WM_RBUTTONDOWN || GetAsyncKeyState(VK_ESCAPE))
			{
				return;
			}
		}
		FlushBatchDraw();
	}
}
void countDown(time_t st, int* countdown)
{
	time_t et = time(NULL);
	int difvalue = et - st;
	RECT re{ 10, 10, 55, 50 };
	if (*countdown > 5)	settextcolor(BLACK);//文字颜色
	else settextcolor(RED);
	settextstyle(20, 20, "微软雅黑");
	setbkmode(TRANSPARENT);
	char time[10] = " ";
	sprintf(time, "%d", *countdown);
	drawtext(time, &re, DT_CENTER);
	*countdown = 60 - difvalue;
}
void fail()
{
	Sleep(100);
	ExMessage mouse;
	while(1)
	{
		BeginBatchDraw();
		setfillcolor(RGB(255, 215, 0));
		solidrectangle(0, 0, getwidth(), getheight());
		RECT re{ 0, 0, getwidth(), getheight() };
		settextcolor(BLACK);
		settextstyle(getheight() - 10, getwidth() / 4 - 140, "微软雅黑");
		setbkmode(TRANSPARENT);
		drawtext("游戏失败", &re, DT_CENTER);
		FlushBatchDraw();
		peekmessage(&mouse);
		if (mouse.message == WM_LBUTTONDOWN || GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE))
		{
			return;
		}
	}
	
}
void nextfloor()
{
	ExMessage mouse;
	Sleep(100);
	while(1)
	{
		BeginBatchDraw();
		setfillcolor(RGB(255, 215, 0));
		solidrectangle(0, 0, getwidth(), getheight());
		RECT re{ 0, 0, getwidth(), getheight() };
		settextcolor(BLACK);
		settextstyle(getheight() - 10, getwidth() / 5 - 130, "微软雅黑");
		setbkmode(TRANSPARENT);
		drawtext("进入下一层", &re, DT_CENTER);
		FlushBatchDraw();
		peekmessage(&mouse);
		if (mouse.message == WM_LBUTTONDOWN || GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE))
		{
			return;
		}
	}
}
void target(const int t)
{
	setfillcolor(RGB(255, 215, 0));
	RECT re{ 10, 45, 300, 75};
	settextcolor(BLACK);
	settextstyle(20, 13, "微软雅黑");
	setbkmode(TRANSPARENT);
	char tar[30] = " ";
	sprintf(tar, "TAR: $%d", t);
	drawtext(tar, &re, DT_LEFT);
}
void current(const int coin)
{
	setfillcolor(RGB(255, 215, 0));
	RECT re{ 10, 60, 300, 90 };
	settextcolor(BLACK);
	settextstyle(20, 13, "微软雅黑");
	setbkmode(TRANSPARENT);
	char cur[30] = " ";
	sprintf(cur, "CUR: $%d", coin);
	drawtext(cur, &re, DT_LEFT);
}
void currentFloor(const int floor)
{
	setfillcolor(RGB(255, 215, 0));
	RECT re{ getwidth() - 350, 45, getwidth(), 75};
	settextcolor(BLACK);
	settextstyle(23, 16, "微软雅黑");
	setbkmode(TRANSPARENT);
	char cur[30] = " ";
	sprintf(cur, "FLOOR:%d", floor);
	drawtext(cur, &re, DT_LEFT);
}
void showTool(const tool *t,const image* img)
{
	if (t->totem >= 1)
	{
		putimage(512, 37, &img->maskImage[11], SRCAND);
		putimage(512, 37, &img->originImage[11], SRCPAINT);
	}
	putimage(833, 87, &img->maskImage[12], SRCAND);
	putimage(833, 87, &img->originImage[12], SRCPAINT);
	setbkmode(TRANSPARENT);
	settextstyle(25, 10, "微软雅黑");
	settextcolor(BLACK);
	RECT rect = { 820, 135, 886, 176 };
	char message[15];
	sprintf(message, "%d", t->ppt);
	drawtext(message, &rect, DT_CENTER);
}
void showHelp()
{
	while (1)
	{
		BeginBatchDraw();
		setfillcolor(RGB(255, 215, 0));
		solidrectangle(0, 0, getwidth(), getheight());
		char message[4][100] = { "1.按ESC退出或暂停", "2.按鼠标左键或键盘↓键放钩", "3.倒计时结束时超过target或将\n屏幕所有矿物抓空进入下一层", "4.当游戏结束时玩家排名会被记录" };;
		settextcolor(BLACK);
		setbkmode(TRANSPARENT);
		for (int i = 0; i < 4; i++)
		{
			settextstyle(55, 26, "微软雅黑");
			RECT ret{ 10, 150 * (i + 1), getwidth() - 10, 150 * (i + 2)};
			drawtext(message[i], &ret, DT_CENTER);
			RECT copyright{ 5, 5, getwidth() - 5, 30};
			settextstyle(26, 15, "微软雅黑");
			drawtext("音乐由[乌鸦Producer]提供", &copyright, DT_CENTER);
		}
		FlushBatchDraw();
		ExMessage mouse;
		peekmessage(&mouse);
		if (mouse.message == WM_LBUTTONDOWN || mouse.message == WM_RBUTTONDOWN || GetAsyncKeyState(VK_ESCAPE))
			break;
	}
}
void showShop(image *IMG, struct tool *t, struct player *p)
{
	BeginBatchDraw();
	char message[50];
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(0, 0, getwidth(), getheight());
	putimage(50, (getheight() - 192) / 2 - 100, &IMG->maskImage[8], SRCAND);
	putimage(50, (getheight() - 192) / 2 - 100, &IMG->originImage[8], SRCPAINT);
	RECT rect{ 50 - 30, (getheight() - 192) / 2 - 100 + 192 + 20, 50 + 192 + 30, (getheight() - 192) / 2 - 100 + 192 + 50 + 20 };
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 20, "微软雅黑");
	drawtext("不死图腾", &rect, DT_CENTER);
	strcpy(message, "＄2000");
	drawButton(20, 414, 272, 462, message);
	putimage(455, 191, &IMG->maskImage[9], SRCAND);
	putimage(455, 191, &IMG->originImage[9], SRCPAINT);
	rect = { 434, 375, 670, 434 };
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 20, "微软雅黑");
	drawtext("大力药水", &rect, DT_CENTER);
	strcpy(message, "＄1000");
	drawButton(418, 414, 683, 462, message);
	putimage(844, 191, &IMG->maskImage[10], SRCAND);
	putimage(844, 191, &IMG->originImage[10], SRCPAINT);
	rect = { 815, 375, 1040, 434 };
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 20, "微软雅黑");
	drawtext("幸运彩票", &rect, DT_CENTER);
	strcpy(message, "＄5000");
	drawButton(795, 414, 1059, 462, message);

	setfillcolor(RED);
	solidrectangle(973, 636, 1037, 700);
	rect = { 973, 636, 1037, 700 };
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(70, 45, "微软雅黑");
	drawtext("X", &rect, DT_CENTER);

	rect = { 30, 30, 350, 93 };
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 10, "微软雅黑");
	sprintf(message, "CUR:＄%d", p->coin);
	drawtext(message, &rect, DT_LEFT);
	FlushBatchDraw();
}

void shopFail()
{
	BeginBatchDraw();
	setfillcolor(BLACK);
	solidrectangle(275, 181, 792, 531);
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(280, 186, 787, 526);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT rect{ 278, 307, 785, 400};
	settextstyle(90, 55, "微软雅黑");
	drawtext("购买失败", &rect, DT_CENTER);
	FlushBatchDraw();
	Sleep(1000);
}

void shopSuccess()
{
	BeginBatchDraw();
	setfillcolor(BLACK);
	solidrectangle(275, 181, 792, 531);
	setfillcolor(RGB(255, 215, 0));
	solidrectangle(280, 186, 787, 526);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT rect{ 278, 307, 785, 400 };
	settextstyle(90, 55, "微软雅黑");
	drawtext("购买成功", &rect, DT_CENTER);
	FlushBatchDraw();
	Sleep(1500);
}

void showLottery(image *img, int price)
{
	ExMessage mouse;
	FlushBatchDraw();
	while (1)
	{
		BeginBatchDraw();
		putimage(0, 0, &img->bk[1]);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT rect{ 180, 275, 900, 540 };
		settextstyle(200, 45, "微软雅黑");
		drawtext("点击刮奖！！！", &rect, DT_CENTER);
		FlushBatchDraw();
		peekmessage(&mouse);
		if (mouse.message == WM_LBUTTONDOWN)
			break;
	}
	//刮奖
	BeginBatchDraw();
	putimage(0, 0, &img->bk[1]);
	char message[10];
	sprintf(message, "＄%d!", price);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT rect{ 180, 275, 900, 540 };
	settextstyle(200, 40, "微软雅黑");
	drawtext(message, &rect, DT_CENTER);
	FlushBatchDraw();
	Sleep(1000);

}

void showTotemBreak(image *IMG)
{
	BeginBatchDraw();
	setfillcolor(BLACK);
	solidrectangle(0, 0, getwidth(), getheight());
	putimage(477, 260, &IMG->maskImage[8], SRCAND);
	putimage(477, 260, &IMG->originImage[8], SRCPAINT);
	FlushBatchDraw();
	totemBreakBgm();
	Sleep(500);

	BeginBatchDraw();
	setfillcolor(BLACK);
	solidrectangle(0, 0, getwidth(), getheight());
	putimage(477, 260, &IMG->maskImage[14], SRCAND);
	putimage(477, 260, &IMG->originImage[14], SRCPAINT);
	FlushBatchDraw();

	Sleep(2000);

	BeginBatchDraw();
	setfillcolor(WHITE);
	solidrectangle(0, 0, getwidth(), getheight());
	putimage(477, 260, &IMG->maskImage[15], SRCAND);
	putimage(477, 260, &IMG->originImage[15], SRCPAINT);
	FlushBatchDraw();

	Sleep(1500);
}