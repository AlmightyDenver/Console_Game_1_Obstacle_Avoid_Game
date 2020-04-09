#include <stdio.h>
#include <stdlib.h>
#include <conio.h>//kbhit()
#include <time.h> //rand()
#include "Console.h"

#define MAX 20
#define BACKCOLOUR 0

struct ST_OBJ
{
	int x = 0;
	int y = 0;
	bool bActive = false;
}; //c++11 st.

ST_OBJ g_stStar[MAX];
ST_OBJ g_stPlayer;


//============================= 초기 설정 =============================//
void Init() {
	g_stPlayer.x = 15;
	g_stPlayer.y = 28;
	g_stPlayer.bActive = true;
	srand(time(NULL));
	SetConsoleSize(30, 30);
}


//============================= 입력 처리 =============================//
void InputProcessor() {
	if (GetAsyncKeyState(VK_LEFT) && 0x8000) {
		g_stPlayer.x-=2;
		if (g_stPlayer.x < 0) g_stPlayer.x = 0;
	}
	if (GetAsyncKeyState(VK_RIGHT) && 0X8000) {
		g_stPlayer.x+=2;
		if (g_stPlayer.x > 28) g_stPlayer.x = 28;
	}

}


//============================= 별 생성 =============================//
void SpawnStar() {
	for (int i = 0; i < MAX; i++) {
		if (!g_stStar[i].bActive) {
			g_stStar[i].x = (rand() % 15) * 2;
			g_stStar[i].y = 0;
			g_stStar[i].bActive = true;
			break;
		}
	}
}


//============================= 별 처리 =============================//
int g_iScore = 0;
int StarProcessor() {
	int cnt = 0;
	for (int i = 0; i < MAX; i++) {
		if (g_stStar[i].bActive) {
			cnt++;
			SetColor(BACKCOLOUR, i % 15);
			GotoXY(g_stStar[i].x, g_stStar[i].y);
			printf("☆");
			g_stStar[i].y++;
			//crash
			if (g_stStar[i].y == g_stPlayer.y && g_stStar[i].x == g_stPlayer.x) {
				g_stPlayer.bActive = false;
			}
			//At End point
			if (g_stStar[i].y > g_stPlayer.y) {
				g_stStar[i].bActive = false;
				if(g_stPlayer.bActive) g_iScore++;
				
			}
		}

	}
	return cnt;
}


//============================= 화면 표시 Rendering =============================//
void Update() {
	if (g_stPlayer.bActive) {
		SetColor(BACKCOLOUR, 11);
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("▲");
	}
	else {
		GotoXY(g_stPlayer.x, g_stPlayer.y);
		printf("X");
		SetColor(BACKCOLOUR, 15);
		GotoXY(10, 9);
		printf("게임 종료");
	}
	SetColor(BACKCOLOUR, 15);
	GotoXY(0, 0);
	printf("Score : %d ", g_iScore);
}


//============================= 시작 화면 =============================//
void StartMenu() {
	//입력 버퍼 getch로 빠져나옴. 재시작때
	while (_kbhit()) _getch();

	while (true) {
		Clear();
		GotoXY(10, 9);
		printf("별 피하기");
		GotoXY(0, 20);
		printf("시작하려면 아무 키나 누르세요");
		Sleep(50);

		if (_kbhit())break;
	}
}


//============================= 게임 화면 =============================//
void GameMain() {
	int state = 0;
	g_stPlayer.x = 15;
	g_stPlayer.y = 28;
	g_stPlayer.bActive = true;
	g_iScore = 0;

	while (true)
	{
		Clear();
		if (g_stPlayer.bActive) {
			//make star
			SpawnStar();
			//Input
			InputProcessor();
		}

		//별 움직임
		state = StarProcessor();
		
		//Render player
		Update();
		Sleep(50);
		if (state == 0) break;
	}
	return;
}


//============================= 결과 화면 =============================//
bool ResultMenu() {
	bool contin = false;
	while (true) {
		Clear();
		GotoXY(10,9);
		printf("최종 점수 : %d", g_iScore);
		GotoXY(0, 20);
		printf("계속하시겠습니까... Y/N ");
		if (GetAsyncKeyState('Y') & 0x8000) {
			contin = true;
			break;
		}
		if (GetAsyncKeyState('N') & 0x8000) {
			contin = false;
			break;
		}
		Sleep(50);
	}
	return contin;
}




//============================= Main =============================//
int main(void) {
	int state = 0;
	Init();
	while (true) {
		StartMenu();
		GameMain();
		if (!ResultMenu()) break;
	}

	system("pause");
	return 0;
}