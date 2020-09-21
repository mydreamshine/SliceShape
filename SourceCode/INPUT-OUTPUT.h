#pragma once
#include "ShapeList.h"

#define Win_WIDTH 800
#define Win_HEIGHT 600
#define Win_DEPTH 800
#define FPS 30

//문자 출력
void drawBitmapText(char *str, D3POINT_f Point, COLORREF_f Color);

//콤보유효시간 (Sec)
#define ComboInterval 2
#define FeverInterval 2
class GAME_INFO
{
private:
	size_t Combo;
	size_t FeverCombo;
	COLORREF_f TextColor;
	D3POINT_f ComboPoint;
	D3POINT_f FeverComboPoint;
	COLORREF_f IntervalColor;
	size_t Count_ComboTime;
	size_t Count_FeverTime;
public:
	GAME_INFO();
	~GAME_INFO() {};

	void Add_Combo();
	void Set_ComboColor(COLORREF_f Color);
	void Set_ComboPos(D3POINT_f Point);
	void Check_Combointerval();
	void Draw_info(CUBE_f Region);
};

// 윈도우 출력 함수
void drawScene(GLvoid);

// 윈도우 갱신(크기 변경 시)
void Reshape(int w, int h);

//키보드 입력
void processNormalKeys(unsigned char input_Char, int Mouse_x, int Mouse_y);
//키보드 입력(키가 눌렸다가 때졌을 때)
void processNormalKeysUp(unsigned char input_Char, int Mouse_x, int Mouse_y);

//마우스 입력(버튼)
void Mouse(int button, int state, int x, int y);
//마우스 이동
void Motion(int x, int y);

//타이머 설정
void TimerFunction(int value);