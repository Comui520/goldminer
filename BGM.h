#pragma once
#include "common.h"

void Bgm()
{
	mciSendString("open ./BGM/JR.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
}
void elongateBgm()
{
	mciSendString("close elongate", 0, 0, 0);
	mciSendString("open ./BGM/出勾.mp3 alias elongate", 0, 0, 0);
	mciSendString("play elongate", 0, 0, 0);
}
void contractBgm()
{
	mciSendString("close contract", 0, 0, 0);
	mciSendString("open ./BGM/收勾.mp3 alias contract", 0, 0, 0);
	mciSendString("play contract", 0, 0, 0);

}
void scoreBgm()
{
	mciSendString("close score", 0, 0, 0);
	mciSendString("open ./BGM/得分.mp3 alias score", 0, 0, 0);
	mciSendString("play score", 0, 0, 0);
}
void totemBreakBgm()
{
	mciSendString("close break", 0, 0, 0);
	mciSendString("open ./BGM/totembreak.mp3 alias break", 0, 0, 0);
	mciSendString("play break", 0, 0, 0);
}
void pptBgm()
{
	mciSendString("close wil", 0, 0, 0);
	mciSendString("open ./BGM/whatislove.mp3 alias wil", 0, 0, 0);
	mciSendString("play wil", 0, 0, 0);
}