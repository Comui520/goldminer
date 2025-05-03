#pragma once
#include "common.h"
#include "BGM.h"
void loadImg(image *img)
{
	for (int i = 0; i < 3; i++)
	{
		char add[100] = " ";
		sprintf(add, "./images/0%d.png", i);//»Æ½ð
		loadimage(img->originImage + i, add);
		sprintf(add, "./images/y0%d.png", i);
		loadimage(img->maskImage + i, add);
	}
	for (int i = 1; i <= IMG_COUNT - 3; i++)
	{
		char add[100] = " ";
		sprintf(add, "./images/%d.png", i);
		loadimage(&img->originImage[2 + i], add);
		sprintf(add, "./images/y%d.png", i);
		loadimage(&img->maskImage[2 + i], add);
	}
	loadimage(&img->bk[0], "./images/bk.jpg", getwidth(), getheight());//±³¾°
	loadimage(&img->bk[1], "./images/²ÊÆ±.jpg", getwidth(), getheight());//±³¾°

}
void iniButton(button* B, int x, int y, int ex, int ey, char message[])
{
	B->x = x;
	B->y = y;
	B->end_x = ex;
	B->end_y = ey;
	strcpy(B->message, message);
}
void iniPlayer(int X, int Y, int W, int H, int C, state s, float po, bool Ppt, struct player* p)
{
	p->x = X;
	p->y = Y;
	p->w = W;
	p->h = H;
	p->coin = C;
	p->st = s;
	p->power = po;
	p->ppt = Ppt;
}
void iniTool(struct tool* t)
{
	t->totem = 0;
	t->ppt = 0;
	t->lottery = 0;
	t->price[0] = 2000;
	t->price[1] = 1000;
	t->price[2] = 5000;
}
void iniHook(double L, double X, double Y, double EX, double EY, double Angle, double ssp, bool f, double S, double CX, double CY, state St, bool cd, struct hook* h)
{
	h->leng = L;
	h->x = X;
	h->y = Y;
	h->end_x = EX;
	h->end_y = EY;
	h->angle = Angle;
	h->swingSpeed = ssp;
	h->flag = f;
	h->speed = S;
	h->cx = CX;
	h->cy = CY;
	h->st = St;
	h->CD = cd;
}

void iniNodeFormine(int t, double X, double Y, int w, int h, bool f, double sp, int p, nfm* n)
{
	n->type = t;
	n->x = X;
	n->y = Y;
	n->width = w;
	n->height = h;
	n->flag = f;
	n->speed = sp;
	n->price = p;
}
void HookMove(hook* h)
{
	if (h->st == H_swing)
	{
		h->end_x = h->x + sin(h->angle) * h->leng;
		h->end_y = h->y + cos(h->angle) * h->leng;
		if (h->flag == true && h->angle <= pi / 2)
			h->angle += h->swingSpeed * pi;
		else if (h->flag == true && h->angle > pi / 2)
			h->flag = false;
		else if (h->flag == false && h->angle >= -(pi / 2))
			h->angle -= h->swingSpeed * pi;
		else if (h->flag == false && h->angle < -(pi / 2))
			h->flag = true;
	}
}
void keyOp(hook* h, nfm* mine, player* p, tool *t)
{
	ExMessage mouse;
	peekmessage(&mouse);
	//ÅÐ¶ÏÊÇ·ñÊ¹ÓÃÒ©Ë®
	if (mouse.message == WM_LBUTTONDOWN && t->ppt > 0 && p->power == 1 && mouse.x >= 835 && mouse.y >= 85 && mouse.x <= 870 && mouse.y <= 130)
	{
		t->ppt--;
		pptBgm();
		p->power = 1.5;
		p->ppt = true;
	}
	//ÅÐ¶ÏÊÇ·ñÊÇ¹´ÎïÌå
	peekmessage(&mouse);
	if ((GetAsyncKeyState(VK_DOWN) && h->CD) || ((mouse.message == WM_LBUTTONDOWN && h->CD) && !(mouse.x >= 835 && mouse.y >= 85 && mouse.x <= 870 && mouse.y <= 130)))
	{
		elongateBgm();
		p->st = H_elongate;
		h->st = H_elongate;
		h->CD = false;
		h->speed = 1.4;
		h->cx = h->speed * sin(h->angle) * p->power;
		h->cy = h->speed * cos(h->angle) * p->power;
	}
	if (h->st == H_elongate)
	{
		h->end_x += h->cx;
		h->end_y += h->cy;
	}
	else if (h->st == H_contract)
	{
		
		if (mine != NULL && mine->flag == true)
		{
			mine->x -= h->cx * mine->speed * p->power;
			mine->y -= h->cy * mine->speed * p->power;
			h->end_x -= h->cx * mine->speed * p->power;
			h->end_y -= h->cy * mine->speed * p->power;
		}
		else
		{
			h->end_x -= h->cx;
			h->end_y -= h->cy;
		}
		if (sqrt(pow(h->end_x - h->x, 2) + pow(h->end_y - h->y, 2)) <= h->leng)
		{
			h->st = H_swing;
			p->st = H_swing;
			h->CD = true;
			if (mine != NULL && mine->flag == true)
			{
				mine->flag = false;
				scoreBgm();
				p->coin += mine->price;
				deleteNode(mine);
				mine = NULL;
			}
		}
	}
	if (h->end_x < 0 || h->end_x > getwidth() || h->end_y < 0 || h->end_y > getheight())
	{
		contractBgm();
		h->st = H_contract;
		p->st = H_contract;
	}
}
nfm* catchMine(hook* h, nfm* head, player* pla)
{
	nfm* p = head->next;
	while (p != NULL)
	{
		if (pow(h->end_x - (p->x + p->width / 2), 2) + pow(h->end_y - (p->y + p->height / 2), 2) <= pow(p->height / 2, 2))
		{
			switch (p->type)
			{
			case 0:
				p->speed = 0.12;
				break;
			case 1:
				p->speed = 0.19;
				break;
			case 2:
				p->speed = 0.26;
				break;
			case 3:
				p->speed = 0.26;
				break;
			case 4:
				p->speed = 0.16;
				break;
			case 5:
				p->speed = 0.26;
				break;
			default:
				break;
			}
			h->st = H_contract;
			pla->st = H_contract;
			h->CD = false;
			p->flag = true;
			break;
		}
		p = p->next;
	}
	return p;
}

nfm* ininFm()
{
	nfm* head = (nfm*)calloc(1, sizeof(nfm));
	iniNodeFormine(-1, -1, -1, -1, -1, 0, 0, 0, head);
	head->next = NULL;
	head->parent = NULL;
	return head;
}
void randomMine(nfm* head, int n)//Í·½Úµã,¹Ø¿¨Êý
{
	nfm* p = head;
	int increment = 0;
	if (n < 5)
	{
		increment += n;
	}
	else if (n >= 5)
	{
		increment = 5;
	}
	for (int i = 0; i < mine_count + increment; i++)
	{
		bool over = true;
		nfm* newnode = NULL;
		int type = 0;
		while (over)
		{
			over = false;
			if (n != 0 && n % 5 == 0)
				type = rand() % 6;
			else
				type = rand() % 5;
			if (type == 0 || type == 3 || type == 4)//»Æ½ð
			{
				int size = rand() % 3;
				if (size == 0)
				{
					newnode = (nfm*)calloc(1, sizeof(nfm));
					iniNodeFormine(0, rand() % (1080 - 179), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 161 - 60), 179, 161, 0, 0, 150, newnode);
				}
				
				else if (size == 1)
				{
					newnode = (nfm*)calloc(1, sizeof(nfm));
					iniNodeFormine(1, rand() % (1080 - 118), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 106 - 60), 118, 106, 0, 0, 120, newnode);

				}
				else
				{
					newnode = (nfm*)calloc(1, sizeof(nfm));
					iniNodeFormine(2, rand() % (1080 - 59), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 53 - 60), 59, 53, 0, 0, 90, newnode);
				}
			}
			else if (type == 1)//¸£´ü
			{
				newnode = (nfm*)calloc(1, sizeof(nfm));
				iniNodeFormine(3, rand() % (1080 - 54), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 62 - 60), 54, 62, 0, 0, 80 + rand() % 20, newnode);

			}
			else if (type == 2)//Ê¯Í·
			{
				newnode = (nfm*)calloc(1, sizeof(nfm));
				iniNodeFormine(4, rand() % (1080 - 73), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 62 - 60), 73, 62, 0, 0, 10, newnode);
			}
			else if (type == 5)//×êÊ¯
			{
				newnode = (nfm*)calloc(1, sizeof(nfm));
				iniNodeFormine(5, rand() % (1080 - 40), (70 + 120) + 60 + rand() % (720 - (70 + 120) - 40 - 60), 40, 40, 0, 0, 200, newnode);
			}
			nfm* ps = NULL;//Ë«Ö¸Õë
			ps = head->next;
			int nx = newnode->x;
			int ny = newnode->y;
			int nex = newnode->x + newnode->width;
			int ney = newnode->y + newnode->height;
			while (ps != NULL)
			{
				int px = ps->x;
				int py = ps->y;
				int pex = ps->x + ps->width;
				int pey = ps->y + ps->height;
				if (!(nex < px || nx > pex || ney < py || ny > pey))
				{
					over = true;
					delete newnode;	
					break;
				}
				ps = ps->next;
			}
		}
		newnode->next = NULL;
		p->next = newnode;
		newnode->parent = p;
		p = p->next;
	}	
}
bool is_Clear(nfm* head)
{
	if (head->next == NULL) return true;
	else return false;
}
int menuButton()
{
	/*strcpy(message, "START/CONTINUE");
	iniButton(&B[0], 270, 300, 270 * 3, 360, message);
	strcpy(message, "CHART");
	iniButton(&B[1], 270, 410, 270 * 3, 470, message);
	strcpy(message, "QUIT");
	iniButton(&B[2], 270, 520, 270 * 3, 580, message);
	strcpy(message, "SHOP");
	iniButton(&B[3], 270, 630, 270 * 3, 690, message);
	strcpy(message, "?");
	iniButton(&B[4], 1080 - 50, 10, 1080 - 10, 60, message);*/
	ExMessage mouse;
	if (peekmessage(&mouse))
	{
		switch (mouse.message)
		{
		case WM_LBUTTONDOWN:
			Sleep(BUTTONSLEEP);
			if (mouse.x >= 270 && mouse.x <= 270 * 3 && mouse.y >= 300 && mouse.y <= 360)
			{
				return 1;//¿ªÊ¼
			}
			else if (mouse.x >= 270 && mouse.x <= 270 * 3 && mouse.y >= 420 && mouse.y <= 480)
			{
				return 2;//ÅÅÐÐ°ñ
			}
			else if (mouse.x >= 270 && mouse.x <= 270 * 3 && mouse.y >= 540 && mouse.y <= 600)
			{
				return 3;//ÍË³öÓÎÏ·
			}
			else if (mouse.x >= 1030 && mouse.x <= 1070 && mouse.y >= 10 && mouse.y <= 60)
			{
				return 4;//°ïÖú½çÃæ
			}
			break;
		}
	}
	return 0;
}
void sortChart(int scor, char name[])
{
	FILE* f = readChart();
	playerOnchart pot[11];
	if (f != NULL)
	{
		int i = 0;
		while (!feof(f) && i != 10)
		{
			fscanf(f, "%d %s %d", &pot[i].rank, pot[i].name, &pot[i].score);
			i++;
		}
	}
	fclose(f);
	if (scor >= pot[9].score)
	{
		pot[10].score = scor;
		strcpy(pot[10].name, name);
	}
	std::sort(pot, pot + 11, cmp);
	for (int i = 0; i < 10; i++)
	{
		pot[i].rank = i + 1;
	}
	f = writeChart();
	if (f != NULL)
	{
		for (int i = 0; i < 10; i++)
			fprintf(f, "%d %s %d\n", pot[i].rank, pot[i].name, pot[i].score);
	}
	fclose(f);
}
int clearanceCondition(nfm* head)
{
	if (head == NULL)
	{
		return 0;
	}
	nfm* p = NULL;
	p = head->next;
	int result = 0;
	while (p != NULL)
	{
		if (p->type == 0 || p->type == 1 || p->type == 2)
			result += p->price * 0.8;
		else if (p->type == 3)
			result += p->price * 0.7;
		else if (p->type == 5)
			result += p->price * 0.1;//×êÊ¯´¿´¿µÄ½±Àø
		p = p->next;
	}
	return result;
}
int shop(struct tool* t, struct player* p)
{
	int item = 0;
	/*
	drawButton(20, 414, 272, 462, message);
	drawButton(418, 414, 683, 462, message);
	drawButton(795, 414, 1059, 462, message);
	*/
	//¼ì²âÊó±êµã»÷
	
	ExMessage mouse;
	while (1)
	{
		peekmessage(&mouse);
		if (mouse.message == WM_LBUTTONDOWN)
			break;
	}
	item = 0;
	if (mouse.x >= 20 && mouse.x <= 272 && mouse.y >= 414 && mouse.y <= 462)
	{
		item = 1;//²»ËÀÍ¼ÌÚ
	}
	else if (mouse.x >= 418 && mouse.x <= 683 && mouse.y >= 414 && mouse.y <= 462)
	{
		item = 2;//´óÁ¦Ò©Ë®
	}
	else if (mouse.x >= 795 && mouse.x <= 1059 && mouse.y >= 414 && mouse.y <= 462)
	{
		item = 3;//²ÊÆ±
	}
	else if (mouse.x >= 973 && mouse.x <= 1037 && mouse.y >= 636 && mouse.y <= 700)
	{
		item = 4;//ÍË³ö
	}
	else
	{
		item = 0;
	}
	//¿ÛÇ®¼ÓÎïÆ·
	switch (item)
	{
	case 1:
		if (p->coin >= t->price[0])
		{
			p->coin -= t->price[0];
			t->totem++;
			return 1;
			break;
		}
		else
		{
			//¹ºÂòÊ§°Ü
			return 2;
			break;
		}
		break;
	case 2:
		if (p->coin >= t->price[1])
		{
			p->coin -= t->price[1];
			t->ppt++;
			return 1;
			break;
		}
		else
		{
			//¹ºÂòÊ§°Ü
			return 2;
			break;
		}
		break;
	case 3:
		if (p->coin >= t->price[2])
		{
			p->coin -= t->price[2];
			t->lottery++;
			return 1;
			break;
		}
		else
		{
			return 2;
			break;
			//¹ºÂòÊ§°Ü
		}
		break;
	case 4://ÍË³öÉÌµê
		return 0;
		break;
	default:
		//¹ºÂòÊ§°Ü
		break;
	}
}
void iniAllButton(button B[BUTTONCOUNT])
{
	char message[100];
	strcpy(message, "START/CONTINUE");
	iniButton(&B[0], 270, 300, 270 * 3, 360, message);
	strcpy(message, "CHART");
	iniButton(&B[1], 270, 420, 270 * 3, 480, message);
	strcpy(message, "QUIT");
	iniButton(&B[2], 270, 540, 270 * 3, 600, message);
	strcpy(message, "?");
	iniButton(&B[3], 1080 - 50, 10, 1080 - 10, 60, message);
}
void iniPptState(player* p)
{
	p->ppt = false;
	p->power = 1;
}
int scratchTickets(player* p, tool* t)
{
	int rando = 0;
	t->lottery--;
	rando = rand() % 921 + 1;
	if (rando == 1)
	{
		p->coin += 100000;
		return 100000;
	}
	else if (rando > 1 && rando <= 21)
	{
		p->coin += 10000;
		return 10000;
	}
	else if (rando > 21 && rando <= 322)
	{
		p->coin += 6000;
		return 6000;
	}
	else if (rando > 322 && rando <= 723)
	{
		p->coin += 5000;
		return 5000;
	}
	else
	{
		p->coin += 1000;
		return 1000;
	}
}
bool totem(int item)
{
	if (item > 0)	return true;
	else return false;
}