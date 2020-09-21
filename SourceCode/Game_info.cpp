#include "INPUT-OUTPUT.h"

//문자 출력
void drawBitmapText(char *str, D3POINT_f Point, COLORREF_f Color)
{
	glPushMatrix();
	glRasterPos3f(Point.x, Point.y, Point.z); //문자열이 그려질 위치 지정
	glColor3f(Color.R, Color.G, Color.B);
	while (*str)
	{
		//GLUT_BITMAP_TIMES_ROMAN_24 폰트를 사용하여 문자열을 그린다.
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);

		str++;
	}
	glPopMatrix();
}


GAME_INFO::GAME_INFO()
{
	Combo = 0;
	FeverCombo = 0;
	TextColor = { 0.0f, };
	ComboPoint = { 0.0f, };
	FeverComboPoint = {0.0f, };
	Count_ComboTime = 0;
	Count_FeverTime = 0;
}

void GAME_INFO::Add_Combo()
{
	Combo++;
	Count_ComboTime = 0;
}

void GAME_INFO::Set_ComboColor(COLORREF_f Color)
{
	TextColor = Color;
	IntervalColor = { (1.0f - Color.R) / (FPS * ComboInterval), (1.0f - Color.G) / (FPS * ComboInterval), (1.0f - Color.B) / (FPS * ComboInterval) };
}

void GAME_INFO::Set_ComboPos(D3POINT_f Point)
{
	ComboPoint = Point;
}

void GAME_INFO::Check_Combointerval()
{
	if (Count_ComboTime >= FPS * ComboInterval)
	{
		Combo = 0;
		FeverCombo = 0;
		Count_ComboTime = 0;
		TextColor = { 0.0f,0.0f,0.0f };
	}
	else if (Combo > 0)
	{
		Count_ComboTime++;
		if (Count_FeverTime != 0)
		{
			FeverComboPoint.y -= 30.0f / (FPS * ComboInterval);
			Count_FeverTime++;
		}
		if (Count_FeverTime >= FPS * FeverInterval) Count_FeverTime = 0;
		TextColor.R += IntervalColor.R;
		TextColor.G += IntervalColor.G;
		TextColor.B += IntervalColor.B;
		ComboPoint.y -= 30.0f / (FPS * ComboInterval);
	}
}

void GAME_INFO::Draw_info(CUBE_f Region)
{
	if (Combo != 0)
	{
		char Str[10] = "x Combo!";
		char Num[10] = { NULL, };
		char Result[20] = { NULL, };
		_itoa_s(Combo, Num, sizeof(Num), 10);
		strcat_s(Result, sizeof(Result), Num);
		strcat_s(Result, sizeof(Result), Str);

		glColor3f(TextColor.R, TextColor.G, TextColor.B);
		drawBitmapText(Result, ComboPoint, TextColor);

		if (Combo >= 30 && FeverCombo != Combo)
		{
			Count_FeverTime = 0;
			Count_FeverTime++;
			FeverCombo = Combo;

			FeverComboPoint = { Region.right / 2, Region.bottom / 2, 0.0f };
		}
		else if (Combo >= 20 && FeverCombo != Combo)
		{
			Count_FeverTime = 0;
			Count_FeverTime++;
			FeverCombo = Combo;

			FeverComboPoint = { Region.right / 2, Region.bottom / 2, 0.0f };
		}
		else if (Combo >= 10 && FeverCombo != Combo)
		{
			Count_FeverTime = 0;
			Count_FeverTime++;
			FeverCombo = Combo;

			FeverComboPoint = { Region.right / 2, Region.bottom / 2, 0.0f };
		}

		if (Count_FeverTime != 0)
		{
			if (FeverCombo >= 30)
			{
				glColor3f(0.0f, 1.0f, 1.0f);
				drawBitmapText("Excellent!", FeverComboPoint, { 0.0f,1.0f,1.0f });
			}
			else if (FeverCombo >= 20)
			{
				glColor3f(1.0f, 0.0f, 1.0f);
				drawBitmapText("Great!", FeverComboPoint, { 1.0f,0.0f,1.0f });
			}
			else if (FeverCombo >= 10)
			{
				glColor3f(1.0f, 0.5f, 0.0f);
				drawBitmapText("Good!", FeverComboPoint, { 1.0f,0.5f,0.0f });
			}
		}
	}
}