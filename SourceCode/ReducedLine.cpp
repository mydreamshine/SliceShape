#include "D3PointList.h"

void Draw_Circle(float CPointX, float CPointY, float r, COLORREF_f Color = { 0.0f,0.0f,0.0f })
{
	float x_ = 0.0f, y_ = 0.0f;
	float nSegement = 100;
	float Angle = 0.0f;
	glColor3f(Color.R, Color.G, Color.B);
	glBegin(GL_TRIANGLE_FAN);
	for (size_t i = 0; i <= nSegement; i++)
	{
		Angle = i * 2.0f * PI / nSegement;
		x_ = CPointX + r * cosf(Angle);
		y_ = CPointY + r * sinf(Angle);
		glVertex3f(x_, y_, 0.0f);
	}
	glEnd();
}

ReducedLine::ReducedLine()
{
	P1 = { 0.0f,0.0f,0.0f };
	P2 = { 0.0f,0.0f,0.0f };
	Speed = 0.0f;
	LineWidth = 0.0f;
}

void ReducedLine::Set_Speed(float Speed)
{
	this->Speed = Speed;
}
void ReducedLine::Set_P1(D3POINT_f Point)
{
	P1 = Point;
}
void ReducedLine::Set_P2(D3POINT_f Point)
{
	P2 = Point;
}
void ReducedLine::Set_LineWidth(float Width)
{
	LineWidth = Width;
}

D3POINT_f ReducedLine::Get_P1()
{
	return P1;
}
D3POINT_f ReducedLine::Get_P2()
{
	return P2;
}

float ReducedLine::Get_dx()
{
	return P2.x - P1.x;
}
float ReducedLine::Get_dy()
{
	return P2.y - P1.y;
}

void ReducedLine::Reduced()
{
	float rad = atan2f(Get_dy(), Get_dx());
	P1.x += Speed * cosf(rad);
	P1.y += Speed * sinf(rad);
	if (Speed * cosf(rad) > 0.0f && P1.x > P2.x)
		P1.x = P2.x;
	else if (Speed * cosf(rad) < 0.0f && P1.x < P2.x)
		P1.x = P2.x;
	if (Speed * sinf(rad) > 0.0f && P1.y > P2.y)
		P1.y = P2.y;
	else if (Speed * sinf(rad) < 0.0f && P1.y < P2.y)
		P1.y = P2.y;
}

void ReducedLine::Draw()
{
	float dx = Get_dx();
	float dy = Get_dy();
	size_t Division = 150;
	COLORREF_f Color = { 0.0f, 0.0f, 0.0f };
	float CPointX = 0.0f, CPointY = 0.0f;
	float r = 0.0f;
	for (size_t i = 0; i < Division; i++)
	{
		Color.R = 1.0f - (float)(i + 1) / (float)Division;
		Color.G = 1.0f - (float)(i + 1) / (float)Division;
		Color.B = 1.0f - (float)(i + 1) / (float)Division;
		CPointX = P1.x + (dx / Division) * i;
		CPointY = P1.y + (dy / Division) * i;
		r = ((LineWidth / Division) * (i + 1)) / 2;
		Draw_Circle(CPointX, CPointY, r, Color);
	}
}