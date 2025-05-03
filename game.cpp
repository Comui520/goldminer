#include "common.h"
#include "graphic.h"
#include "function.h"
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
int main()
{
	image IMG;
	int floor = 1;
	bool clear = false;
	srand(unsigned int(time(NULL)));
	button B[BUTTONCOUNT];
	iniAllButton(B);
	initgraph(1080, 720);
	loadImg(&IMG);
	HWND h = GetHWnd();
	SetWindowText(h, "ª∆ΩøÛπ§");
	Bgm();
	while (1)
	{
		player p;
		iniPlayer((getwidth() - 140) / 2, 70, 140, 120, 0, H_swing, 1, false, &p);//≥ı ºªØÕÊº“
		tool t;
		iniTool(&t);
		floor = 1;
		while (1)
		{
			BeginBatchDraw();
			menu(B);
			int tmp = menuButton();
			if (tmp == 3)
				return 0;
			else if (tmp == BUTTONCOUNT)
			{
				showHelp();
			}
			else if (tmp == 2)
			{
				showChart();
			}
			else if (tmp == 1)
			{
				break;
			}
			FlushBatchDraw();
		}
		InputBox(p.name, 45, "«Î ‰»Îƒ˙µƒ√˚◊÷", " ‰»Î√˚◊÷", NULL, 400, 0);
		//bool exi = true;
		//while (exi)
		//{
		//	showShop(&IMG, &t, &p);
		//	switch (shop(&t, &p))
		//	{
		//	case 0://ÕÀ≥ˆ
		//		exi = false;
		//		break;
		//	case 2://π∫¬Ú ß∞‹
		//		shopFail();
		//		break;
		//	case 1:
		//		shopSuccess();
		//		break;
		//	}
		//}
		while (1)
		{
			hook h;
			iniHook(60, p.x + 45, p.y + 90, p.x + 45, p.y + 150, 0.0, 0.0009, true, 0.0, 0.0, 0.0, H_swing, true, &h);
			iniPptState(&p);
			while (t.lottery > 0)
			{
				int price = scratchTickets(&p, &t);
				showLottery(&IMG, price);
			}
			int countdown = 60;
			nfm* curMove = NULL;
			nfm* head = ininFm();
			randomMine(head, floor);
			int clearcondition = clearanceCondition(head) + p.coin;
			time_t start_time = time(NULL);
			while (countdown > 0 && !is_Clear(head))
			{
				BeginBatchDraw();
				drawPlayer(&IMG, &p, h);
				countDown(start_time, &countdown);
				currentFloor(floor);

				current(p.coin);
				target(clearcondition);
				showTool(&t, &IMG);
				keyOp(&h, curMove, &p, &t);
				HookMove(&h);
				//ppt(&p, &t);
				drawMine(head, &IMG);
				if (catchMine(&h, head, &p) != NULL)
				{
					curMove = catchMine(&h, head, &p);
				}
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					time_t tmp1 = time(NULL);
					FlushBatchDraw();
					while (1)
					{
						BeginBatchDraw();
						menu(B);
						int tmp = menuButton();
						if (tmp == 3)
						{
							sortChart(p.coin, p.name);
							return 0;
						}
						else if (tmp == BUTTONCOUNT)
						{
							showHelp();
						}
						else if (tmp == 2)
						{
							showChart();
						}
						else if (tmp == 1)
						{
							break;
						}
						FlushBatchDraw();
					}
					time_t tmp2 = time(NULL);
					start_time -= tmp1 - tmp2;
				}
				sleep_us(150);//”√‘›Õ£øÿ÷∆÷°¬ 
				FlushBatchDraw();
			}
			if (p.coin < clearcondition)
			{
				if (totem(t.totem))//”–ÕºÃ⁄≤ªÀ¿
				{
					showTotemBreak(&IMG);
					t.totem--;
				}
				else//√ªÕºÃ⁄À¿Õˆ
				{
					fail();
					sortChart(p.coin, p.name);
					break;
				}
			}
			floor++;
			if (floor - 1 != 0 && (floor - 1) % 5 == 0)
			{
				bool exi = true;
				while (exi)
				{
					showShop(&IMG, &t, &p);
					switch (shop(&t, &p))
					{
					case 0://ÕÀ≥ˆ
						exi = false;
						break;
					case 2://π∫¬Ú ß∞‹
						shopFail();
						break;
					case 1:
						shopSuccess();
						break;
					}
				}

			}
			nextfloor();
		}
	}
	closegraph();
	return 0;
}
